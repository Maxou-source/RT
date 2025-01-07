#ifndef Tuple_HPP
# define Tuple_HPP

typedef float	t_f4 __attribute__((ext_vector_type(4)));
// # include <iostream>
# include <string>
# include <iostream>
# include <math.h>


// # include "Poçint.hpp"
/*
adding tuples
subsracting tuples
negating tuples
scalar multiplication and division
magnitude

point + point -> point
point + vector -> point
vector + vector -> vector

sphere_to_ray ← ray.origin - point(0, 0, 0)
a ← dot(ray.direction, ray.direction)
b ← 2 * dot(ray.direction, sphere_to_ray)
c ← dot(sphere_to_ray, sphere_to_ray) - 1
discriminant ← b² - 4 * a *
*/

class Tuple {
	protected:
		t_f4 value;
	public:

	// constructor qnd Destructors
		Tuple();
		Tuple(float x, float y, float z, float w);
		~Tuple();

	// setters and getters
		t_f4	getValue();

	// overload operators
		Tuple	operator+(const Tuple&) const;
		Tuple	operator-(const Tuple&) const;
		Tuple	operator*(float t);

	// methods
		float dot_product(Tuple &);
		void display();
		float magnitude();
		void normalize();
		Tuple normalize(Tuple&);
};

#endif