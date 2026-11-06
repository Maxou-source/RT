#include "Pattern.hpp"
#include "ParseUtils.hpp"

Pattern::Pattern() {
}

bool Pattern::setProperty(const std::string& key, const std::string& val) {
	try {
		if (key == "type") {return parsePatternType(val);}
		else if (key == "parameter") { param = std::stof(val);}
		else if (key == "second_color") {return ParseUtils::parseColor(val, clr);}
		else if (key == "orientation") {return ParseUtils::parseVec(val, orientation, 1.0);}
	} catch (const std::exception&) {return false;}
	return true;
}

bool Pattern::preBuild()  {
	// if (type == GRADIENT)
	// 	return true;
	Matrix scale_Matrix;
	scale_Matrix.scale(param, param, param);
	scale_Matrix.display();

	Matrix orientation_matrix;
	orientation_matrix.orientation(orientation.x, orientation.y,orientation.z);
	orientation_matrix.display();
	i_m = orientation_matrix * scale_Matrix;
	i_m = i_m.invertedMatrix().transpose();
	return true;
}

std::vector<std::string> Pattern::requiredKeys() const {
	return {"type", "parameter", "second_color", "orientation"};
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
	g.pat.i_m = i_m.getMatrix();
	return ;
}

bool Pattern::parsePatternType(const std::string& value) {
	std::unordered_map<std::string, PatternType> map = {
		{"checker", PatternType::CHECKER}, {"stripe", PatternType::STRIPE},
		{"gradient", PatternType::GRADIENT}, {"perlin", PatternType::PERLIN}, 
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
