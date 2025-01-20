#include "Intersection.hpp"

/*======= Constructors and Destructors =======*/

Intersection::Intersection() { }
Intersection::Intersection(float tt) {
	t = tt;
}

Intersection::Intersection(float tt, AObject *o) {
	t = tt;
	obj = o;
}

/*===== Overload Operators =======*/

bool Intersection::operator<(const Intersection &other) const {
	if (t < 0 && other.t >= 0) return false;
	if (other.t < 0 && t >= 0) return true;
	return t < other.t;
}

std::ostream& operator<<(std::ostream& os, const std::set<Intersection>& intersection){
	std::set<Intersection>::iterator it;
	for (it = intersection.begin(); it != intersection.end(); it++)
	{
		os << "inter " << (*it).getT() << "\n";
	}
	return os;
}
/*===== Setters and Getters ======*/

float Intersection::getT() const {return t;}
void Intersection::setT(float tmp) {t = tmp;}

AObject	*Intersection::getObjPtr() const {return obj;}
void	Intersection::setObjPtr(AObject *o) {obj = o;}
