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

Tuple::Tuple(t_f4 f) :value(f) {}

Tuple::Tuple(const Tuple& t) {value = t.value;}

Tuple::~Tuple() {

}

/*=== Overload Operators====*/

Tuple  Tuple::operator+(const Tuple &t) const
{
    return Tuple(
        this->value.x + t.value.x,
        this->value.y + t.value.y,
        this->value.z + t.value.z,
        this->value.w + t.value.w
    );
}

Tuple  Tuple::operator-(const Tuple &t) const
{
    return Tuple(
        this->value.x - t.value.x,
        this->value.y - t.value.y,
        this->value.z - t.value.z,
        this->value.w - t.value.w
    );
}

Tuple Tuple::operator*(float t) const{
	Tuple tu(value.x * t, value.y * t, value.z * t, value.w *t);
	return (tu);
}

Tuple Tuple::operator*(const Tuple& t) const {
	return Tuple(value * t.value);
}

Tuple Tuple::operator*(const Matrix& m) const {
	t_m4 ma = m.getMatrix();
	return Tuple(ma[0][0] * value.x + ma[0][1] * value.y + ma[0][2] * value.z + ma[0][3] * value.w,
				ma[1][0] * value.x + ma[1][1] * value.y + ma[1][2] * value.z + ma[1][3] * value.w,
				ma[2][0] * value.x + ma[2][1] * value.y + ma[2][2] * value.z + ma[2][3] * value.w,
				ma[3][0] * value.x + ma[3][1] * value.y + ma[3][2] * value.z + ma[3][3] * value.w
	);
}

bool	equal(float a, float b)
{
	if (fabs(a - b) < EPSILON)
		return (true);
	return (false);
}


bool	Tuple::operator==(const Tuple& t) const
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (!equal(t.value[i], value[i]))
			return (false);
		i++;
	}
	return (true);
}


std::ostream& operator<<(std::ostream& os, const Tuple& tu) {
	t_f4 g = tu.getValue();
	os << g.x << " " << g.y << " " << g.z << " " << g.w << std::endl;
	return os;
}

/*===== Getters and Setters ====*/
t_f4	Tuple::getValue() const {
	return value;
}

/*========Methods=========*/

Tuple Tuple::reflect(const Tuple &in, const Tuple &normalv)
{
	Tuple reflectv =  in - normalv * 2.0f * in.dot_product(normalv);
	return reflectv;
}

void Tuple::display()
{
	std::cout << "Tuple : " << "x: " << value.x
	<< " y: " << value.y << " z: " << value.z << " w: " << value.w << std::endl;
}

float Tuple::dot_product(const Tuple& t) const
{
	return (
		(value.x * t.value.x) +
		(value.y * t.value.y) +
		(value.z * t.value.z) +
		(value.w * t.value.w) 
	);
}

Tuple Tuple::cross_product(const Tuple& t) const
{
	t_f4	res;

	res.x = value.y * t.value.z - value.z * t.value.y;
	res.y = value.z * t.value.x - value.x * t.value.z;
	res.z = value.x * t.value.y - value.y * t.value.x;
	res.w = 0;
	Tuple ret(res);
	return (ret);
}

float Tuple::magnitude() const
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

Tuple Tuple::normalize(const Tuple& fix)
{
	float magnitude = fix.magnitude();
	return Tuple(fix.value.x / magnitude,
				fix.value.y / magnitude,
				fix.value.z / magnitude,
				fix.value.w / magnitude);
}

Tuple Tuple::negating(const Tuple& t)
{
	return Tuple(-t.value.x, -t.value.y, -t.value.z, -t.value.w);
}

void Tuple::negating()
{
	value = -value;
}
