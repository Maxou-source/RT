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

/*===== Setters and Getters =======*/

void	AObject::setPosition(t_f4 p) {position = p;}
void	AObject::setSpecular(const float s) {specular = s;}
void	AObject::setMaterial(const Material& m) {material = m;}
void	AObject::setPattern(const bool b) {pattern = b;}

t_f4	AObject::getPosition() const {return position;}
float	AObject::getSpecular() const {return specular;}
const Material&	AObject::getMaterial() const {return material;}
bool	AObject::getPattern() const {return pattern;}
