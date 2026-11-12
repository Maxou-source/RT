#include "Torus.hpp"

Torus::Torus() {

}

void Torus::printSpecific(std::ostream& os) const
{
	os << "  A:   " << A << "\n";
	os << "  B:   " << B << "\n";
}

std::vector<std::string> Torus::requiredKeys() const {
	return ParseUtils::concat(AObject::requiredKeys(), {"orientation", "A", "B"});
}

const char * Torus::typeString() const {
	return "Torus";
}

bool Torus::packIntoGpuScene(GpuScene& s) const {
	if (!AObject::packIntoGpuScene(s))
		return false;

	GpuObject& g = s.objs[s.object_count - 1];
	g.param = A;
	g.param2 = B;
	return true;
}

bool Torus::preBuild() {
	// Matrix scale_Matrix;
	// scale_Matrix.scale(A, A, A);
	// scale_Matrix.display();

	Matrix translation_matrix;
	translation_matrix.translate(position.x, position.y, position.z);
	translation_matrix.display();

	Matrix orientation_matrix;
	orientation_matrix.orientation(orientation.x, orientation.y,orientation.z);
	orientation_matrix.display();
	
	i_m = translation_matrix * orientation_matrix ; //* scale_Matrix;
	i_m = i_m.invertedMatrix().transpose();
	std::cout << "displaying finale" << std::endl;
	i_m.display();
	return true;
}

bool Torus::setProperty(const std::string& key, const std::string& val) {
	if (key == "A")
	{
		try { A = std::stof(val); } catch (...) { return false; }
		return true;
	}
	else if (key == "B")
	{
		try { B = std::stof(val); } catch (...) { return false; }
		return true;
	}
	else if (key == "orientation") return parseVec(val, orientation, 0.f);
	return AObject::setProperty(key, val);
	return true;
}

