#ifndef DATABASE_H
#define DATABASE_H

#include <fstream>
#include <yaml-cpp/yaml.h>
#include <openssl/md5.h>
#include <sys/stat.h>

#include "encryptor.h"

struct Key
{
    std::string name;
    std::string identity;
    std::string password;
    std::string more;
};

class Database
{
private:
    bool _opened;
    std::string _oldFilename;
    YAML::Node _rootNode;
    Encryptor* _encryptor;

    std::vector<std::string> split(const std::string& str, char delimiter);

public:
    Database(const std::string& filename);
    ~Database();

    std::string filename() const;

    bool open(const std::string& password);

    bool isOpened() const;

    bool nodeTo();

    YAML::Node nodeContent(const std::string& route = "");
    YAML::Node subNode(YAML::Node& node, std::vector<std::string>& groups);

    void operator[](const std::string& route);
    void addKeyNode(const std::string& route, const Key& key);
    void addKeyNode(
        const std::string& route,
        const std::string& name,
        const std::string& identity,
        const std::string& password,
        const std::string& more
        );
    void removeNode(const std::string& route);

    bool save();
    bool save(const std::string& filename);
};

#endif // DATABASE_H
