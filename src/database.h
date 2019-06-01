#ifndef DATABASE_H
#define DATABASE_H

#include <fstream>
#include <yaml-cpp/yaml.h>
#include <openssl/md5.h>
#include <sys/stat.h>
#include <algorithm>

#include "encryptor.h"

struct Node
{
    std::string name;

    std::vector<Node> children;
    std::string identity;
    std::string password;
    std::string more;

    bool isGroup() const
    {
        return identity.empty();
    }
};

class Database
{
private:
    bool _opened;
    std::string _oldFilename;
    Node _rootNode;
    Encryptor* _encryptor;

    std::vector<std::string> split(const std::string& str, char delimiter);

    Node loadNode(const YAML::Node& yamlNode);
    YAML::Node saveNode(const Node& node);

public:
    Database(const std::string& filename);
    ~Database();

    std::string filename() const;

    bool open(const std::string& password);

    bool isOpened() const;

    bool nodeTo();

    Node& nodeContent(const std::string& route = "");
    Node& subNode(Node& node, std::vector<std::string>& groups);

    void addKeyNode(
        const std::string& route,
        const std::string& name,
        const std::string& identity,
        const std::string& password,
        const std::string& more
        );

    std::string parentRoute(const std::string& route) const;

    void removeNode(const std::string& route);

    bool save();
    bool save(const std::string& filename);
};

#endif // DATABASE_H
