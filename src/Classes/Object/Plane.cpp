#include "Plane.hpp"

Plane::Plane() {

}

void Plane::printSpecific(std::ostream& os) const {
	os << " "; 
}

std::vector<std::string> Plane::requiredKeys() const {
	return ParseUtils::concat(AObject::requiredKeys(), {"orientation"});
}

const char * Plane::typeString() const {
	return "Plane";
}

bool Plane::packIntoGpuScene(GpuScene& s) const {
	if (!AObject::packIntoGpuScene(s))
		return false;
	return true;
}

bool Plane::setProperty(const std::string& key, const std::string& val) {
	if (key == "orientation") return parseVec(val, orientation, 0.f);
	return AObject::setProperty(key, val);
	return true;
}

bool Plane::preBuild() {
	Matrix translation_matrix;
	translation_matrix.translate(position.x, position.y, position.z);
	translation_matrix.display();

	Matrix orientation_matrix;
	orientation_matrix.orientation(orientation.x, orientation.y,orientation.z);
	orientation_matrix.display();
	
	i_m = translation_matrix * orientation_matrix;
	i_m = i_m.invertedMatrix().transpose();
	std::cout << "displaying finale" << std::endl;
	i_m.display();
	return true;
}


bool Plane::repeatableSubBlock() const {
	return false;
}
