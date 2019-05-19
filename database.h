#ifndef DATABASE_H
#define DATABASE_H

#include <fstream>
#include <yaml-cpp/yaml.h>

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

    std::vector<std::string> split(const std::string& str, char delimiter);

public:
    Database(const std::string& filename);

    std::string filename() const;

    bool open();

    bool isOpened() const;

    bool nodeTo();

    YAML::Node groupNode(const std::string& route);
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

    YAML::Node nodeContent(const std::string& route);

    bool save();
    bool save(const std::string& filename);
};

#endif // DATABASE_H
