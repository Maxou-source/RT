#pragma once

# include <string>
# include "rt.hpp"

namespace ParseUtils
{
	std::string trim(const std::string& s);
	bool parseFloat(const std::string& v, float& out);
	bool parseBool(const std::string& v, bool& out);
	bool parseColor(const std::string& v, t_clr& out);
	bool parseVec(const std::string& v, t_f4& out, float w);
	bool parsePosition(const std::string& v, t_f4& out);   // parseVec(..., 1.f)
	bool parseDirection(const std::string& v, t_f4& out);  // parseVec(..., 0.f)
	std::vector<std::string> concat(std::vector<std::string> a,
                                       const std::vector<std::string>& b);
}