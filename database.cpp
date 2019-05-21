#include "database.h"

Database::Database(const std::string& filename)
  : _opened(false),
    _oldFilename(filename),
    _encryptor(nullptr)
{

}

Database::~Database()
{
    if(_encryptor)
        delete _encryptor;
}

std::vector<std::string> Database::split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
        if(token != "")
            tokens.push_back(token);
    }
    return tokens;
}

std::string Database::filename() const
{
    return _oldFilename;
}

bool Database::open(const std::string& password)
{
    if(isOpened())
        return false;

    std::ifstream filestream(_oldFilename);

    std::stringstream yamlStream;
    yamlStream << filestream.rdbuf();

    char* key = (char*) MD5((const unsigned char*)password.c_str(), password.size(), NULL);
    std::string iv = password;
    iv.resize(16, '0');

    if(_encryptor)
        delete _encryptor;

    _encryptor = new Encryptor(key, iv);

    if(!_encryptor->isInitialized())
        return false;

    std::string decryptedstr;
    if(!_encryptor->decrypt(yamlStream.str(), decryptedstr))
    {
        _opened = false;
        delete _encryptor;
        _encryptor = nullptr;
    }
    else
    {
        _opened = true;
        _rootNode =  YAML::Load(decryptedstr);
    }

    return isOpened();
}

bool Database::isOpened() const
{
    return _opened;
}

YAML::Node Database::nodeContent(const std::string& route)
{
	std::vector<std::string> groups = split(route, '/');

	YAML::Node node = _rootNode;
	if(groups.size() > 0)
        return subNode(node, groups);

	return node;
}

YAML::Node Database::subNode(YAML::Node& node, std::vector<std::string>& groups)
{
    if(groups.size() > 0)
    {
        bool found = false;
        YAML::Node childNode;

        for(size_t i = 0; i < node["children"].size(); i++)
        {
            if(node["children"][i]["name"].as<std::string>() == groups[0])
            {
                childNode = node["children"][i];
                found = true;
                break;
            }
        }

        if(!found)
        {
            childNode["name"] = groups[0];
            node["children"].push_back(childNode);
        }

        groups.erase(groups.begin());
        return subNode(childNode, groups);
    }
    else
    {
        return node;
    }
}

void Database::operator[](const std::string& route)
{
	nodeContent(route);
}

void Database::addKeyNode(const std::string& route, const Key& key)
{
	addKeyNode(route, key.name, key.identity, key.password, key.more);
}

void Database::addKeyNode(
	const std::string& route,
    const std::string& name,
    const std::string& identity,
    const std::string& password,
    const std::string& more
    )
{
	YAML::Node node = nodeContent(route);

	node[name]["identity"] = identity;
	node[name]["password"] = password;
	node[name]["more"] = more;
}

void Database::removeNode(const std::string& route)
{
	std::vector<std::string> groups = split(route, '/');

    size_t i = 0;
	YAML::Node node = _rootNode;
	for(i = 0; i < groups.size()-1; i++)
	{
		node = node[groups[i]];
	}

	node.remove(groups[i]);
}

bool Database::save()
{
	return save(_oldFilename);
}

bool Database::save(const std::string& filename)
{
    if(!isOpened())
        return false;

    std::stringstream yamlStream;
	yamlStream << _rootNode;

    std::string encryptedstr;

    if(!_encryptor->encrypt(yamlStream.str(), encryptedstr))
        return false;

    std::ofstream fout(filename);
	fout << encryptedstr;

    return true;
}