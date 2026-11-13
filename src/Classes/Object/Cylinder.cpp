# include "Cylinder.hpp"
//cy -0.5,-5,-33 0,1,0 5 20 255,140,0

Cylinder::Cylinder()
{
}

std::vector<std::string> Cylinder::requiredKeys() const {
	return ParseUtils::concat(AObject::requiredKeys(), {"orientation", "diameter", "height"});
}

void Cylinder::printSpecific(std::ostream& os) const
{
	os << "  diameter:   " << diameter << "\n"
	   << "  height:     " << height << "\n";
}

bool Cylinder::setProperty(const std::string& key, const std::string& val)
{
	if (key == "height")
	{
		try { height = std::stof(val); } catch (...) { return false; }
		return true;
	}
	else if (key == "diameter")
	{
		try { diameter = std::stof(val); } catch (...) { return false; }
		return true;
	}
	else if (key == "orientation") return parseVec(val, orientation, 0.f);
	return AObject::setProperty(key, val);
}

const char * Cylinder::typeString() const {
	return "Cylinder";
}

bool Cylinder::preBuild() {
	Matrix scale_Matrix;
	scale_Matrix.scale(diameter, 1.0, diameter);
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

bool Cylinder::packIntoGpuScene(GpuScene& s) const {
	if (!AObject::packIntoGpuScene(s))
		return false;

	GpuObject& g = s.objs[s.object_count - 1];
	g.param = height;
	return true;
}

bool Cylinder::repeatableSubBlock() const {
	return false;
}

