#include "AObject.hpp"

int AObject::idCounter = 0;

/*=====Constructors and Destructors======*/

AObject::AObject() {
	id = ++idCounter;
	center = 0;
	m.display();
	i_m = m.invertedMatrix();
}

/*===== Methods =====*/

void AObject::scale(float x, float y, float z)
{
	m.scale(x, y, z);
	i_m = m.invertedMatrix();
}

/*function reflect(in, normal)
return in - normal * 2 * dot(in, normal)*/

void AObject::reflect(const Tuple &in)
{
	reflectv =  in - normalv * 2 * in.dot_product(normalv);
}

/*===== Setters and Getters =======*/

void		AObject::setMatrix(Matrix &ma) {m = ma;}
void		AObject::setIMatrix(Matrix &ma) {i_m = ma;}
Matrix&		AObject::getMatrix() {return m;};
Matrix&		AObject::getIMatrix() {return i_m;};