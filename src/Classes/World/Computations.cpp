#include "Computations.hpp"
#include "Ray.hpp"
#include "AObject.hpp"
/*====== Construcors and Destructors=====*/

Computations::Computations() {}

Computations::Computations(const Intersection& inter, Ray *r) {
	t = inter.getT();
	obj = inter.getObjPtr();
	point = r->position(t);
	eyev = Tuple::negating(r->getDirection());
	normalv = obj->normal(point);
	if (eyev.dot_product(normalv) < 0)
	{
		inside = true;
		normalv.negating();
	}
	else
		inside = false;
}

/*====== Overload Operator ======*/

std::ostream& operator<<(std::ostream& os, const Computations& comps) {
	os << "Computations:" << "\n" 
		<< "t      : "<< comps.getT() << std::endl
		<< "point  : "<< comps.getPoint()
		<< "eyev   : "<< comps.getEyev()
		<< "normal : "<< comps.getNormalv() << std::endl;
	return os;
}

/*====== Setters and Getters ========*/

float			Computations::getT() const {return t;}
void			Computations::setT(float tt) {t = tt;}

AObject*		Computations::getObjPtr() const {return obj;}
void			Computations::setObjPtr(AObject *o) {obj = o;}

const Tuple&	Computations::getPoint() const {return point;}
void			Computations::setPoint(const Tuple& p) { point = p;}

const Tuple&	Computations::getEyev() const {return eyev;}
void			Computations::setEyev(const Tuple& ey) {eyev = ey;}

const Tuple&	Computations::getNormalv()const{return normalv;}
void			Computations::setNormalv(const Tuple&n) {normalv = n;}