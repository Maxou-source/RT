# include "Cube.hpp"

Cube::Cube() {

}

void Cube::printSpecific(std::ostream& os) const
{
	os << "  size:   " << size << "\n";
}

std::vector<std::string> Cube::requiredKeys() const {
	return ParseUtils::concat(AObject::requiredKeys(), {"orientation", "size"});
}

const char * Cube::typeString() const {
	return "Cube";
}

bool Cube::packIntoGpuScene(GpuScene& s) const {
	if (!AObject::packIntoGpuScene(s))
		return false;

	GpuObject& g = s.objs[s.object_count - 1];
	g.param = size;
	return true;
}

bool Cube::preBuild() {
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

bool Cube::repeatableSubBlock() const {
	return false;
}


bool Cube::setProperty(const std::string& key, const std::string& val) {
	if (key == "size")
	{
		try { size = std::stof(val); } catch (...) { return false; }
		return true;
	}
	else if (key == "orientation") return parseVec(val, orientation, 0.f);
	return AObject::setProperty(key, val);
	return true;
}


