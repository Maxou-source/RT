
#ifndef PARSE_HPP
# define PARSE_HPP

# include "rt.hpp"
# include "World.hpp"
# include "IParsable.hpp"

using IterVectorString = std::vector<std::string>::iterator;

class Parse {
	private:
		World						*w;
		std::vector<std::string>	lines;
	public:
		Parse();
		bool parseFileAndName(const char *);
		std::vector<std::string>	parseContent(std::ifstream *fileStream);

		bool	checkFileName(const char *fileName);
		bool	checkFileRights(const char *fileName, std::ifstream *filestream);

		bool	getBase();
		bool	parseBlock(IterVectorString& it, IterVectorString end, IParsable& target);

		World	*GetMyWorld();
};

#endif