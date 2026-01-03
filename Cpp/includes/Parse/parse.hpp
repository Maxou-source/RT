
#ifndef PARSE_HPP
# define PARSE_HPP

# include "rt.hpp"

class Parse {
	private:

	public:
		static bool parse(const char *);
		static std::vector<std::string>	parseContent(std::ifstream *fileStream);

		static bool checkFileName(const char *fileName);
		static bool checkFileRights(const char *fileName, std::ifstream *filestream);

};

#endif