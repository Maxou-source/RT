#include "Tuple.hpp"

/*==== Constructors and Destructors ====*/

Tuple::Tuple() {

}

Tuple::Tuple(float x, float y, float z, float w) {
	value.x = x;
	value.y = y;
	value.z = z;
	value.w = w;
}

Tuple::~Tuple() {

}

/*=== Overload Operators====*/

Tuple  Tuple::operator+(const Tuple &fix) const
{
    return Tuple(
        this->value.x + fix.value.x,
        this->value.y + fix.value.y,
        this->value.z + fix.value.z,
        this->value.w + fix.value.w
    );
}

Tuple  Tuple::operator-(const Tuple &fix) const
{
    return Tuple(
        this->value.x - fix.value.x,
        this->value.y - fix.value.y,
        this->value.z - fix.value.z,
        this->value.w - fix.value.w
    );
}

Tuple Tuple::operator*(float t)
{
	value = value * t;
	return (*this);
}

/*===== Getters and Setters ====*/
t_f4	Tuple::getValue() {
	return value;
}

/*Methods*/

void Tuple::display()
{
	std::cout << "Tuple : " << "x: " << value.x
	<< " y: " << value.y << " z: " << value.z << " w: " << value.w << std::endl;
}

float Tuple::dot_product(Tuple& t)
{
	return (
		(value.x * t.value.x) +
		(value.y * t.value.y) +
		(value.z * t.value.z) +
		(value.w * t.value.w) 
	);
}

float Tuple::magnitude()
{
	return (sqrtf((value.x * value.x) + 
					(value.y * value.y) +
					(value.z * value.z) ));
}

void Tuple::normalize()
{
	float magnitude = this->magnitude();
	value.x = value.x / magnitude;
	value.y  = value.y / magnitude;
	value.z  = value.z / magnitude;
	value.w  = value.w / magnitude;
}

Tuple Tuple::normalize(Tuple& fix)
{
	float magnitude = fix.magnitude();
	return Tuple(fix.value.x / magnitude,
				fix.value.y / magnitude,
				fix.value.z / magnitude,
				fix.value.w / magnitude);
}