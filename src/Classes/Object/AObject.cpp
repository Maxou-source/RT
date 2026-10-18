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
	return material.setProperty(key, val);   // color, specular, pattern, ...
}

std::vector<std::string> AObject::requiredKeys() const {
	return {"position"};
}

void packMatrix(const Matrix& m, t_m4& out)
{
	t_m4 ret = m.getMatrix();
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			out[r][c] = static_cast<float>(ret[r][c]);
}

bool AObject::packIntoGpuScene(GpuScene& s) {
	std::cout << "packing"<< std::endl;
	if (s.object_count >= MAX_OBJECTS)
		return false;

	GpuObject& g = s.objs[s.object_count++];     // write directly into the slot
	g = GpuObject{};                             // zero everything, including padding

	packMatrix(i_m, g.inv_transform);
	g.type = static_cast<int>(type());

	g.mat.color[0]   = material.color.r;
	g.mat.color[1]   = material.color.g;
	g.mat.color[2]   = material.color.b;
	g.mat.ambient    = material.ambient;
	g.mat.diffuse    = material.diffuse;
	g.mat.specular   = material.specular;
	g.mat.shininess  = material.shininess;
	g.mat.reflective = material.reflective;
	g.mat.pattern    = material.pattern ? 1 : 0;
	g.mat.texture    = -1;
	return true;
}


/*===== Setters and Getters =======*/

void	AObject::setPosition(t_f4 p) {position = p;}
void	AObject::setSpecular(const float s) {specular = s;}
void	AObject::setMaterial(const Material& m) {material = m;}
void	AObject::setPattern(const bool b) {pattern = b;}

t_f4	AObject::getPosition() const {return position;}
float	AObject::getSpecular() const {return specular;}
const Material&	AObject::getMaterial() const {return material;}
bool	AObject::getPattern() const {return pattern;}
