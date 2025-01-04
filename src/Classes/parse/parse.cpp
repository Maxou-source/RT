#include "parse.hpp"

bool Parse::checkFileName(char *fileName)
{
	std::string argStr = fileName;
	if (argStr.size() < 4)
		return true;
	if (argStr.substr(argStr.size() - 3) != ".rt") {
		return true;
	}
	return false;
}

bool Parse::checkFileRights(char *fileName, std::ifstream *fileStream)
{
	fileStream->open(fileName);
	if (!fileStream->is_open()) {
		return true;
	}
	return false;
}

bool Parse::parseContent(std::ifstream *fileStream)
{
	(void) fileStream;
	std::string					line;
	std::vector<std::string>	vectorFile;

	while (std::getline(*fileStream, line)) {
		// line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		std::cout << line << std::endl;
		vectorFile.push_back(line);
	}
	return (false);
}

bool Parse::parse(char *fileName)
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
	if (parseContent(&fileStream))
	{
		return true;
	}
	return false;
}