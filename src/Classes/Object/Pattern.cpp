#include "Pattern.hpp"
#include "ParseUtils.hpp"

Pattern::Pattern() {
}

bool Pattern::setProperty(const std::string& key, const std::string& val) {
	try {
		if (key == "type") {return parsePatternType(val);}
		else if (key == "parameter") { param = std::stof(val);}
		else if (key == "second_color") {return ParseUtils::parseColor(val, clr);}
	} catch (const std::exception&) {return false;}
	return true;
}

bool Pattern::preBuild()  {
	return true;
}

std::vector<std::string> Pattern::requiredKeys() const {
	return {"type", "parameter", "second_color"};
}

const char *Pattern::typeString() const {
	return "pattern";
}

bool Pattern::packIntoGpuScene(GpuScene&) const {
	return true;
}

void Pattern::packInto(GpuMaterial& g) const {
	std::cout << "packing pattern" << std::endl;
	g.pat.patType = type;
	g.pat.color[0] = clr.r;
	g.pat.color[1] = clr.g;
	g.pat.color[2] = clr.b;
	g.pat.param = param;
	return ;
}

bool Pattern::parsePatternType(const std::string& value) {
	std::unordered_map<std::string, PatternType> map = {
		{"checker", PatternType::CHECKER}, {"stripe", PatternType::STRIPE}, 
	};
	auto it = map.find(value);
	if (it == map.end())
		return false;
	type = it->second;
	return true;
}

// GETTERS AND SETTERS

PatternType Pattern::getType() const {
	return type;
}

float Pattern::getParam() const {
	return param;
}
