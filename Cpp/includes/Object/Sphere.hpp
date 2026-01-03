#include "AObject.hpp"

class Sphere : public AObject {
	public:
	// Constructors and Destructors 
		Sphere();
	// methods
		std::set<Intersection>	intersect(Ray *r);
		Tuple	normal(const Tuple&);
};