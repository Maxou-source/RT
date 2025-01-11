#include "AObject.hpp"

class Sphere : public AObject {
	public:
	// Constructors and Destructors 
		Sphere();
	// methods
		float	intersect(Ray *r);
		Tuple	normal(const Tuple&);
};