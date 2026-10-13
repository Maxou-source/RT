#include "AObject.hpp"

int AObject::idCounter = 0;

/*=====Constructors and Destructors======*/

AObject::AObject() {
	id = ++idCounter;
	center = 0;
	// m.display();
}

/*===== Methods =====*/

void AObject::scale(float x, float y, float z)
{
	std::cout << "hello" << std::endl;
	Matrix tmp;
	tmp.scale(x , y,z);
	m = m * tmp;
}

void AObject::translate(float x, float y, float z)
{
	Matrix b;
	b.translate(x,y,z);
	m = m * b;
}

void AObject::applyTransformations()
{
	i_m = m.invertedMatrix();
	t_m = m.transpose();
}
/*function reflect(in, normal)
return in - normal * 2 * dot(in, normal)*/

// void AObject::reflect(const Tuple &in)
// {
// 	reflectv =  in - normalv * (float)2 * in.dot_product(normalv);
// }

/*===== Setters and Getters =======*/

void		AObject::setMatrix(const Matrix &ma) {m = ma;}
void		AObject::setIMatrix(Matrix &ma) {i_m = ma;}
Matrix&		AObject::getMatrix() {return m;};
Matrix&		AObject::getIMatrix() {return i_m;};
// Tuple AObject::getColor() const {return color;}
// void AObject::setColor(const Tuple& t) { color = t;}

void AObject::setColor(const t_clr& c) {color = c;}
void AObject::setDiameter(const float d) {diameter = d;}
void AObject::setSpecular(const float s) {specular = s;}
void AObject::setPattern(const bool b) {pattern = b;}

t_clr AObject::getColor() const {return color;}
float AObject::getDiameter() const {return diameter;}
float AObject::getSpecular() const {return specular;}
bool  AObject::getPattern() const {return pattern;}
		// t_clr		color;
		// float		diameter;
		// float		specular;
		// bool		pattern;

