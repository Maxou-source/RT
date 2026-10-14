#pragma once

#include <map>
#include <set>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

class ShaderLoader
{
public:
	bool load(const std::string& path,
				const std::map<std::string, std::string>& defines,
				std::string& out);
	const std::vector<std::string>& files() const {return fileTable;}

private:
	bool expand(const std::string& path, std::string& out, int depth);
	static bool parseDirective(const std::string& line, std::string& file);

	std::set<std::string>		included;
	std::vector<std::string>	fileTable;
	static const int			MAX_DEPTH = 16;
};