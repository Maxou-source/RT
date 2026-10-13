#include "parse.hpp"
#include "AObject.hpp"

Parse::Parse() {
	w = new World;
}

bool Parse::checkFileName(const char *fileName)
{
	std::string argStr = fileName;
	if (argStr.size() < 4)
		return true;
	if (argStr.substr(argStr.size() - 3) != ".rt") {
		return true;
	}
	return false;
}

bool Parse::checkFileRights(const char *fileName, std::ifstream *fileStream)
{
	fileStream->open(fileName);
	if (!fileStream->is_open()) {
		return true;
	}
	return false;
}

ObjectId findObjectId(std::string& s)
{
	const std::array<std::string, 3> strings_id = {"None","Sphere", "Cylinder"};
	
	for (size_t i = 1; i < strings_id.size(); i++) {
		if (s.find(strings_id[i]) != std::string::npos)
			return static_cast<ObjectId>(i);
	}
	return ObjectId::NONE;
}

std::string trim(const std::string& s)
{
	size_t posParam = s.find_first_not_of(" \t\r");
	if (posParam == std::string::npos)
		return "";
	size_t posEndParam = s.find_last_not_of(" \t\r");
	return s.substr(posParam, posEndParam - posParam + 1);
}

// bool parseColor(std::string v, float outColor[3])
// {
// 	for (char& c : v)
// 	{
// 		if (c == '[' || c == ']' || c == ',')
// 			c = ' ';
// 		std::istringstream ss(v);
// 		return (ss >> outColor[0] >> outColor[1] >> outColor[2] ? true : false);
// 	}
// 	return false;
// }

bool	Parse::AddObject(IterVectorString& it, IterVectorString end, ObjectId id)
{
	// s'arreter si on arrive a la fin ou si on arrive a un '}'
	AObject *obj = w->AddObject(id);
	++it;
	for (; it != end; ++it) {
		std::string line = trim(*it);
		if (line.empty())
			continue ;
		if (line[0] == '}')
		{
			++it;
			return true;
		}

		size_t posColon = line.find(':');
		if (posColon == std::string::npos)
		{
			std::cout << "KO :: file is not as expected" << std::endl;
			return false;
		}
		std::string key = trim(line.substr(0, posColon));
		std::string value = trim(line.substr(posColon + 1));
		if (!applyProperty(obj, key, value))
		{
			std::cout << "exiting at apply property" << std::endl;
			return false;
		}
	}
	return false;
}

bool Parse::applyProperty(AObject* obj, const std::string& key, const std::string& val)
{
	try
	{
		if (key == "color")
		{
			float c[3];
			// if (!parseColor(val, c))
			// 	return false;
			t_clr clr;
			clr.r = c[0];
			clr.g = c[1];
			clr.b = c[2];
			obj->setColor(clr);
		}
		else if (key == "specular")  obj->setSpecular(std::stof(val));
		else if (key == "diameter")  obj->setDiameter(std::stof(val));
		else if (key == "pattern")   obj->setPattern(val == "true");
		else
		{
			std::cout << "ERROR::PARSE: unknown key '" << key << "'" << std::endl;
			return false;
		}
	}
	catch (const std::exception&)    // stof throws on bad input
	{
		std::cout << "ERROR::PARSE: bad value for '" << key << "': " << val << std::endl;
		return false;
	}
	return true;
}

bool	Parse::getBase()
{
	ObjectId objId = ObjectId::NONE;

	for (IterVectorString it = lines.begin(); it != lines.end(); )
	{
		if (it->empty()) {
			++it;
			continue ;
		}
		objId = findObjectId(*it);
		if (objId != ObjectId::NONE) {
			AddObject(it, lines.end(), objId);
			continue ;
		}
		else
		{
			std::cout << "exit here" << std::endl;
			return false;
		}
	}
	return true;
}

std::vector<std::string>	Parse::parseContent(std::ifstream *fileStream)
{
	(void) fileStream;
	std::string					line;
	std::vector<std::string>	vectorFile;

	while (std::getline(*fileStream, line)) {
		std::cout << line << std::endl;
		vectorFile.push_back(line);
	}
	return vectorFile;
}

bool Parse::parseFileAndName(const char *fileName)
{
	if (checkFileName(fileName))
	{
		std::cerr << "KO: Wrong file extension" << std::endl;
		return true;
	}
	std::ifstream fileStream;
	if (checkFileRights(fileName, &fileStream))
	{
		std::cerr << "KO: Not a file or you dont have the correct rights" << std::endl;
		return true;
	}
	lines = parseContent(&fileStream);
	if (lines.size() == 0)
		return true;
	return false;
}

/*========================= SETTERS AND GETTERS ====================*/

World	*Parse::GetMyWorld() {return w;}