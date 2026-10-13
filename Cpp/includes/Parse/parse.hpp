
#ifndef PARSE_HPP
# define PARSE_HPP

# include "rt.hpp"
# include "World.hpp"

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
		bool	AddObject(IterVectorString& it, IterVectorString end, ObjectId id);
		bool	applyProperty(AObject* obj, const std::string& key, const std::string& val);

		World	*GetMyWorld();
};

#endif