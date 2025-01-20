#ifndef Tuple_HPP
# define Tuple_HPP

# include "rt.hpp"
// # include <iostream>
# include <string>
# include <iostream>
# include <math.h>
# include "Matrix.hpp"


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
		Tuple(const Tuple &);
		Tuple(t_f4 f);
		~Tuple();

	// setters and getters
		t_f4	getValue() const;

	// overload operators
		Tuple	operator+(const Tuple&) const;
		Tuple	operator-(const Tuple&) const;
		Tuple	operator*(float t) const;
		Tuple	operator*(const Tuple &t) const;
		Tuple	operator*(const Matrix &t) const;


	// methods
		float dot_product(const Tuple &) const;
		void display();
		float magnitude() const;
		void normalize();
		void negating();
		static Tuple normalize(const Tuple&);
		static Tuple reflect(const Tuple &in, const Tuple &normalv);

		static Tuple negating(const Tuple& t);
};

std::ostream& operator<<(std::ostream& os, const Tuple&);

#endif