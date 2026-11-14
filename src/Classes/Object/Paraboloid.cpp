#include "Paraboloid.hpp"


Paraboloid::Paraboloid() {

}

void Paraboloid::printSpecific(std::ostream& os) const
{
	os << "  size:   " << size << "\n";
}

std::vector<std::string> Paraboloid::requiredKeys() const {
	return ParseUtils::concat(AObject::requiredKeys(), {"orientation", "size", "height"});
}

const char * Paraboloid::typeString() const {
	return "Paraboloid";
}

bool Paraboloid::packIntoGpuScene(GpuScene& s) const {
	if (!AObject::packIntoGpuScene(s))
		return false;

	GpuObject& g = s.objs[s.object_count - 1];
	g.param = size;
	g.param2 = height;
	return true;
}

bool Paraboloid::preBuild() {
	Matrix scale_Matrix;
	scale_Matrix.scale(size, size, size);
	scale_Matrix.display();

	Matrix translation_matrix;
	translation_matrix.translate(position.x, position.y, position.z);
	translation_matrix.display();

	Matrix orientation_matrix;
	orientation_matrix.orientation(orientation.x, orientation.y,orientation.z);
	orientation_matrix.display();
	
	i_m = translation_matrix * orientation_matrix * scale_Matrix;
	i_m = i_m.invertedMatrix().transpose();
	std::cout << "displaying finale" << std::endl;
	i_m.display();
	return true;
}

bool Paraboloid::repeatableSubBlock() const {
	return false;
}


bool Paraboloid::setProperty(const std::string& key, const std::string& val) {
	if (key == "size")
	{
		try { size = std::stof(val); } catch (...) { return false; }
		return true;
	}
	else if (key == "height") {
		try { height = std::stof(val); } catch (...) { return false; }
		return true;
	}
	else if (key == "orientation") return parseVec(val, orientation, 0.f);
	return AObject::setProperty(key, val);
}


