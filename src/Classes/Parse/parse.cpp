#include "parse.hpp"

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

// bool	getBase(std::vector<std::string> vec)
// {
// 	const std::array<std::string, 2> objects_id = {"Sphere", "Cylinder"};

// 	bool inprogress = false;
// 	for (std::string &s : vec)
// 	{
// 		if (s.empty())
// 			continue ;
// 		if (!inprogress && s.find(objects_id[0]) || s.find(objects_id[1])) {
// 			inprogress = s.[s.size() - 1] == '{';
// 			if (!inprogress)
// 				return false;
// 			continue ;
// 		}
// 		if (inprogress && )
// 	}
// }

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

bool Parse::parse(const char *fileName)
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
	std::vector<std::string> lines = parseContent(&fileStream);
	if (lines.size() > 0)
	{

		return true;
	}
	return false;
}