#include "database.h"

Database::Database(const std::string& filename)
  : _opened(false),
    _oldFilename(filename)
{

}

std::vector<std::string> Database::split(const std::string& str, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(str);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

std::string Database::filename() const
{
    return _oldFilename;
}

bool Database::open()
{
    std::ifstream filestream(_oldFilename);

    std::stringstream yamlStream;
    yamlStream << filestream.rdbuf();

    // TODO decode

    // TODO opened = true / false

    _rootNode =  YAML::Load(yamlStream.str());

    return isOpened();
}

bool Database::isOpened() const
{
    return _opened;
}

YAML::Node Database::groupNode(const std::string& route)
{
	std::vector<std::string> groups = split(route, '/');

	YAML::Node node = _rootNode;
	for(auto group : groups)
	{
		YAML::Node childNode(YAML::NodeType::Sequence);

		node = (node[group] = childNode);
	}

	return node;
}

void Database::operator[](const std::string& route)
{
	groupNode(route);
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
	YAML::Node node = groupNode(route);

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
	std::stringstream yamlStream;
	yamlStream << _rootNode;

    // TODO encode

	std::ofstream fout(filename);
	fout << yamlStream.str();

    return true;
}