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
	const std::array<std::string, 4> strings_id = {"None","Sphere", "Cylinder", "Camera"};
	
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
	for (; it != end; ++it)
	{
		std::string line = trim(*it);
		if (line.empty()) continue;
		if (line[0] == '}') {
			++it;
			for (std::string& k : target.requiredKeys())
			{
				if (!seenKeys.count(k))
				{
					std::cerr << "KO:: missing mandatory property \"" << k << "\" for " << target.typeString() << std::endl;
					return false;
				}
			}
			return true;
		}

		size_t colon = line.find(':');
		if (colon == std::string::npos) return false;
		std::string key = trim(line.substr(0, colon));
		std::string val = trim(line.substr(colon + 1));

		if (seenKeys.count(key))
		{
			std::cerr << "KO:: duplicate properties detected " << std::endl;
			return false;
		}
		if (!target.setProperty(key, val))
		{
			std::cout << "KO :: unknown or invalid property '" << key << "'" << std::endl;
			return false;
		}
		seenKeys.insert(key);
	}
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
		if (objId != ObjectId::NONE) {
			AObject* obj = w->AddObject(objId);
			bool blockResult = obj && parseBlock(it, lines.end(), *obj);
			if (!blockResult){
				std::cerr << "Something wrong with file" << std::endl;
				return false;
			}
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