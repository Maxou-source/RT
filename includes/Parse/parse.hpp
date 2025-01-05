
#ifndef PARSE_HPP
# define PARSE_HPP

# include "rt.hpp"

class Parse {
	private:

	public:
		static bool parse(char *);
		static bool parseContent(std::ifstream *fileStream);

		static bool checkFileName(char *fileName);
		static bool checkFileRights(char *fileName, std::ifstream *filestream);

};

#endif