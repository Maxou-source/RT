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

Tuple Tuple::MatrixTuple(Matrix& m)
{
	t_f4	res;
	t_m4 a = m.getMatrix();

	res.x = a[0][0] * value.x + a[0][1] * value.y + a[0][2] * value.z + a[0][3] * value.w;
	res.y = a[1][0] * value.x + a[1][1] * value.y + a[1][2] * value.z + a[1][3] * value.w;
	res.z = a[2][0] * value.x + a[2][1] * value.y + a[2][2] * value.z + a[2][3] * value.w;
	res.w = a[3][0] * value.x + a[3][1] * value.y + a[3][2] * value.z + a[3][3] * value.w;

	Tuple	ret(res.x, res.y, res.z, res.w);
	return (ret);
}