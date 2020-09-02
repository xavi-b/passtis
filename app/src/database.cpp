#include "database.h"

#include <spdlog/spdlog.h>

Database::Database() : _opened(false), _encryptor(nullptr)
{
}

Database::~Database()
{
    if (_encryptor)
        delete _encryptor;
}

Database* Database::instance()
{
    static Database db;
    return &db;
}

std::vector<std::string> Database::split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string              token;
    std::istringstream       tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
        if (token != "")
            tokens.push_back(token);
    }
    return tokens;
}

std::string Database::filename() const
{
    return _oldFilename;
}

Node Database::loadNode(const YAML::Node& yamlNode)
{
    Node node;
    node.name = yamlNode["name"].as<std::string>();

    if (yamlNode["children"].IsDefined())
    {
        for (size_t i = 0; i < yamlNode["children"].size(); i++)
        {
            Node child = loadNode(yamlNode["children"][i]);
            node.children.push_back(child);
        }
    }
    else
    {
        node.identity = yamlNode["identity"].as<std::string>();
        node.password = yamlNode["password"].as<std::string>();
        node.more     = yamlNode["more"].as<std::string>();
    }

    return node;
}

bool Database::open(const std::string& filename, const std::string& password)
{
    spdlog::get("filelogger")->debug("open file: {}", filename);

    if (isOpened())
        return false;

    _oldFilename = filename;
    std::ifstream filestream(_oldFilename);

    std::stringstream yamlStream;
    yamlStream << filestream.rdbuf();

    char*       key = (char*)SHA256((const unsigned char*)password.c_str(), password.size(), NULL);
    std::string iv  = password;
    iv.resize(16, '0');

    if (_encryptor)
        delete _encryptor;

    _encryptor = new Encryptor(key, iv);

    if (!_encryptor->isInitialized())
        return false;

    struct stat buffer;
    bool        fileExists = (stat(_oldFilename.c_str(), &buffer) == 0);

    std::string decryptedstr;
    if (fileExists && !_encryptor->decrypt(yamlStream.str(), decryptedstr))
    {
        spdlog::get("filelogger")->debug("decrypt error {}", _encryptor->error());

        _opened = false;
        delete _encryptor;
        _encryptor = nullptr;
    }
    else
    {
        _opened             = true;
        YAML::Node yamlNode = YAML::Load(decryptedstr);

        for (size_t i = 0; i < yamlNode.size(); i++)
        {
            Node child = loadNode(yamlNode[i]);
            _rootNode.children.push_back(child);
        }
    }

    spdlog::get("filelogger")->debug("open {}", isOpened());

    return isOpened();
}

bool Database::isOpened() const
{
    return _opened;
}

bool Database::nodeExists(const std::string& route)
{
    return getNode(route) != nullptr;
}

Node* Database::getNode(const std::string& route)
{
    std::vector<std::string> groups = split(route, '/');

    if (groups.size() > 0)
    {
        return getSubNode(&_rootNode, groups);
    }

    return &_rootNode;
}

Node* Database::getSubNode(Node* node, std::vector<std::string>& groups)
{
    if (groups.size() > 0)
    {
        for (size_t i = 0; i < node->children.size(); i++)
        {
            if (node->children[i].name == groups[0])
            {
                groups.erase(groups.begin());
                return getSubNode(&node->children[i], groups);
            }
        }

        return nullptr;
    }
    else
    {
        return node;
    }
}

Node& Database::addGroupNode(const std::string& route)
{
    std::vector<std::string> groups = split(route, '/');

    if (groups.size() > 0)
    {
        return addSubGroupNode(_rootNode, groups);
    }

    return _rootNode;
}

Node& Database::addSubGroupNode(Node& node, std::vector<std::string>& groups)
{
    if (groups.size() > 0)
    {
        for (size_t i = 0; i < node.children.size(); i++)
        {
            if (node.children[i].name == groups[0])
            {
                groups.erase(groups.begin());
                return addSubGroupNode(node.children[i], groups);
            }
        }

        Node childNode;
        childNode.name = groups[0];
        node.children.push_back(childNode);

        groups.erase(groups.begin());
        return addSubGroupNode(node.children[node.children.size() - 1], groups);
    }
    else
    {
        return node;
    }
}

void Database::addKeyNode(const std::string& route,
                          const std::string& name,
                          const std::string& identity,
                          const std::string& password,
                          const std::string& more)
{
    Node& node = addGroupNode(route);

    Node key;
    key.name     = name;
    key.identity = identity;
    key.password = password;
    key.more     = more;

    node.children.push_back(key);
}

std::string Database::parentRoute(const std::string& route) const
{
    return route.substr(0, route.find_last_of('/') + 1);
}

void Database::removeNode(const std::string& route)
{
    std::string r      = route.substr(0, route.find_last_of('/') + 1);
    std::string child  = route.substr(r.find_last_of('/') + 1);
    Node*       parent = getNode(r);
    auto        it     = std::find_if(parent->children.begin(), parent->children.end(), [child](const Node& node) {
        return node.name == child;
    });
    if (it != parent->children.end())
    {
        parent->children.erase(it);
    }
}

YAML::Node Database::saveNode(const Node& node)
{
    YAML::Node yamlNode;
    yamlNode["name"] = node.name;

    if (node.isGroup())
    {
        yamlNode["children"] = YAML::Node(YAML::NodeType::Sequence);
        for (size_t i = 0; i < node.children.size(); i++)
        {
            YAML::Node child = saveNode(node.children[i]);
            yamlNode["children"].push_back(child);
        }
    }
    else
    {
        yamlNode["identity"] = node.identity;
        yamlNode["password"] = node.password;
        yamlNode["more"]     = node.more;
    }

    return yamlNode;
}

bool Database::save()
{
    return save(_oldFilename);
}

bool Database::save(const std::string& filename)
{
    if (!isOpened())
        return false;

    YAML::Node yamlNode;
    for (size_t i = 0; i < _rootNode.children.size(); i++)
    {
        YAML::Node child = saveNode(_rootNode.children[i]);
        yamlNode.push_back(child);
    }

    std::stringstream yamlStream;
    yamlStream << yamlNode;

    std::string encryptedstr;

    if (!_encryptor->encrypt(yamlStream.str(), encryptedstr))
        return false;

    std::ofstream fout(filename);
    fout << encryptedstr;

    return true;
}