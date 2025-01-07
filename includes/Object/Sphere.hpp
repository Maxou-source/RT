#include "AObject.hpp"

class Sphere : public AObject {
	public:
		Sphere();
		bool intersect(Ray *r);
};