#include "parse.hpp"
#include "AObject.hpp"
#include "Light.hpp"

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
	const std::array<std::string, 8> strings_id = {"None","Sphere", "Cylinder", "Cube", "Plane",
		"Cone", "Camera", "Light"};
	
	for (size_t i = 1; i < strings_id.size(); i++) {
		if (s.find(strings_id[i]) != std::string::npos)
			return static_cast<ObjectId>(i);
	}
	return ObjectId::NONE;
}

bool Parse::parseBlock(IterVectorString& it, IterVectorString end, IParsable& target)
{
	std::set<std::string> seenKeys;
	++it;
	while (it != end)
	{
		std::string line = ParseUtils::trim(*it);
		if (line.empty()) { ++it; continue; }

		if (line[0] == '}')
		{
			++it;
			bool ok = true;
			for (const std::string& k : target.requiredKeys())
				if (!seenKeys.count(k))
				{
					std::cerr << "KO:: missing mandatory property \"" << k
					          << "\" for " << target.typeString() << std::endl;
					ok = false;                 // keep going: report every missing key
				}
			return ok && target.preBuild();     // your finalize equivalent
		}

		if (line.back() == '{')
		{
			std::string name = ParseUtils::trim(line.substr(0, line.size() - 1));
			if (!name.empty() && name.back() == ':')
				name.pop_back();
			name = ParseUtils::trim(name);

			IParsable* sub = target.getSubBlock(name);
			if (!sub)
			{
				std::cerr << "KO:: unknown sub-block '" << name << "'" << std::endl;
				return false;
			}
			if (seenKeys.count(name))
			{
				std::cerr << "KO:: duplicate sub-block '" << name << "'" << std::endl;
				return false;
			}
			if (!parseBlock(it, end, *sub))     // leaves `it` after the sub-block's '}'
				return false;
			seenKeys.insert(name);
			continue;                           // no ++it: already on the next line
		}

		size_t colon = line.find(':');
		if (colon == std::string::npos)
		{
			std::cerr << "KO:: expected 'key: value': " << line << std::endl;
			return false;
		}
		std::string key = ParseUtils::trim(line.substr(0, colon));
		std::string val = ParseUtils::trim(line.substr(colon + 1));

		if (seenKeys.count(key))
		{
			std::cerr << "KO:: duplicate property '" << key << "'" << std::endl;
			return false;
		}
		if (!target.setProperty(key, val))
		{
			std::cerr << "KO:: unknown or invalid property '" << key << "'" << std::endl;
			return false;
		}
		seenKeys.insert(key);
		++it;
	}
	std::cerr << "KO:: missing '}'" << std::endl;
	return false;
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
		if (objId == ObjectId::CAMERA)
		{
			Camera& objCamera = w->getCamera();
			bool blockResult = parseBlock(it, lines.end(), objCamera);
			if (!blockResult) {
				return false;
			}
			objCamera.preBuild();
			continue ;
		}
		else if (objId == ObjectId::LIGHT) {
			Light& objLight = w->AddLight();
			if (!parseBlock(it, lines.end(), objLight)) {
				return false;
			}
		}
		else if (objId != ObjectId::NONE) {
			AObject* obj = w->AddObject(objId);
			bool blockResult = obj && parseBlock(it, lines.end(), *obj);
			if (!blockResult){
				std::cerr << "Something wrong with file" << std::endl;
				return false;
			}
			obj->preBuild();
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