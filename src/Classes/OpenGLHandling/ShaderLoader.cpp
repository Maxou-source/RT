#include "ShaderLoader.hpp"

bool ShaderLoader::load(const std::string& path,
                        const std::map<std::string, std::string>& defines,
                        std::string& out)
{
	included.clear();
	fileTable.clear();

	out = "#version 460 core\n";                       // the only place the version lives
	for (const auto& d : defines)
		out += "#define " + d.first + " " + d.second + "\n";

	return expand(path, out, 0);
}

// returns true and fills `file` if the line is  //!include "xxx"
bool ShaderLoader::parseDirective(const std::string& line, std::string& file)
{
	const std::string tag = "//!include";
	size_t s = line.find_first_not_of(" \t");
	if (s == std::string::npos || line.compare(s, tag.size(), tag) != 0)
		return false;
	size_t a = line.find('"', s + tag.size());
	size_t b = (a == std::string::npos) ? a : line.find('"', a + 1);
	if (b == std::string::npos)
		return false;                      // malformed: treat as normal line (or report)
	file = line.substr(a + 1, b - a - 1);
	return true;
}

bool ShaderLoader::expand(const std::string& path, std::string& out, int depth)
{
	if (depth > MAX_DEPTH)
		return std::cerr << "KO:: include depth exceeded at " << path << std::endl, false;

	std::string key = std::filesystem::weakly_canonical(path).string();
	if (!included.insert(key).second)      // insert fails if already present
		return true;

	std::ifstream in(path);
	if (!in)
		return std::cerr << "KO:: cannot open " << path << std::endl, false;

	int id = fileTable.size();
	fileTable.push_back(path);
	out += "#line 1 " + std::to_string(id) + "\n";

	std::string line, file;
	int n = 0;
	while (std::getline(in, line))
	{
		n++;
		if (parseDirective(line, file))
		{
			std::string next = (std::filesystem::path(path).parent_path() / file).string();
			if (!expand(next, out, depth + 1))
				return false;
			out += "#line " + std::to_string(n + 1) + " " + std::to_string(id) + "\n";
		}
		else
			out += line + "\n";
	}
	return true;
}