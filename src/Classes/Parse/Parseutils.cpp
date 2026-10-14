// ParseUtils.cpp
#include "ParseUtils.hpp"
#include <sstream>

namespace ParseUtils
{
	std::string trim(const std::string& s)
	{
		size_t a = s.find_first_not_of(" \t\r");
		if (a == std::string::npos)
			return "";
		size_t b = s.find_last_not_of(" \t\r");
		return s.substr(a, b - a + 1);
	}

	bool parseFloat(const std::string& v, float& out)
	{
		try { out = std::stof(v); }
		catch (const std::exception&) { return false; }
		return true;
	}

	bool parseBool(const std::string& v, bool& out)
	{
		if (v == "true")  { out = true;  return true; }
		if (v == "false") { out = false; return true; }
		return false;
	}

	bool parseColor(const std::string& v, t_clr& out)
	{
		std::string s = v;
		for (char& c : s)
			if (c == '[' || c == ']' || c == ',')
				c = ' ';
		std::istringstream ss(s);
		return static_cast<bool>(ss >> out.r >> out.g >> out.b);
	}

	bool parseVec(const std::string& v, t_f4& out, float w)
	{
		std::string s = v;
		for (char& c : s)
			if (c == '[' || c == ']' || c == ',')
				c = ' ';
		float x, y, z;
		std::istringstream ss(s);
		if (!(ss >> x >> y >> z))
			return false;
		out = t_f4{x, y, z, w};
		return true;
	}

	bool parsePosition(const std::string& v, t_f4& out)  { return parseVec(v, out, 1.f); }
	bool parseDirection(const std::string& v, t_f4& out) { return parseVec(v, out, 0.f); }

	std::vector<std::string> concat(std::vector<std::string> a,
                                       const std::vector<std::string>& b)
	{
		a.insert(a.end(), b.begin(), b.end());
		return a;
	}
}