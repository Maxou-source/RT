#include "Ray.hpp"

// Ray::Ray()  { }

Ray::Ray(const Tuple& p, const Tuple& v) : origin(p), direction(v) {}
// Ray::Ray(Tuple p, Tuple& v) : origin(p), direction(v) {}

Tuple Ray::position(float t) {
	Tuple res;
	res = origin + (direction * t);
	// (void) t;
	// (void) origin;
	// (void) direction;
	return res;
}


/*==== Getters and Setters====*/

Tuple& Ray::getOrigin() {return origin;}
Tuple& Ray::getDirection() {return direction;}
void Ray::setDirection(const Tuple& d) {direction = d;}
void Ray::setOrigin(const Tuple& o) {origin = o;}

// t_f4 Ray::getValue() {return value;}