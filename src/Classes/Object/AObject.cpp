#include "AObject.hpp"

/*=====Constructors and Destructors======*/

AObject::AObject() {
	// m.display();
}

const char * AObject::typeString() const {
	return "AObject";
}

/*===== Methods =====*/

void AObject::applyTransformations()
{
	i_m = m.invertedMatrix();
	t_m = m.transpose();
}

bool AObject::setProperty(const std::string& key, const std::string& val)
{
	if (key == "position")
		return parsePosition(val, position);
	return false;   // color, specular, pattern, ...
}

std::vector<std::string> AObject::requiredKeys() const {
	return {"position", "Material"};
}

void packMatrix(const Matrix& m, t_m4& out)
{
	t_m4 ret = m.getMatrix();
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			out[r][c] = static_cast<float>(ret[r][c]);
}

bool AObject::packIntoGpuScene(GpuScene& s) const {
	std::cout << "packing"<< std::endl;
	if (s.object_count >= MAX_OBJECTS)
		return false;

	GpuObject& g = s.objs[s.object_count++];     // write directly into the slot
	g = GpuObject{};                             // zero everything, including padding

	packMatrix(i_m, g.inv_transform);
	g.type = static_cast<int>(type());

	material.packInto(g.mat);
	return true;
}

IParsable* AObject::getSubBlock(const std::string& name)
{
	if (name == "Material")
		return &material;
	return nullptr;
}
/*===== Setters and Getters =======*/

void	AObject::setPosition(t_f4 p) {position = p;}
void	AObject::setMaterial(const Material& m) {material = m;}

t_f4	AObject::getPosition() const {return position;}
const Material&	AObject::getMaterial() const {return material;}
