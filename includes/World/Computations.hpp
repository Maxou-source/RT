#ifndef COMPUTATIONS_HPP
# define COMPUTATIONS_HPP

# include "Tuple.hpp"
# include "Intersection.hpp"
# include <stdbool.h>

class Ray;
class AObject;

class Computations {
	private:
		float		t;
		AObject*	obj;
		Tuple		point;
		Tuple		eyev;
		Tuple		normalv;
		bool		inside;
	public:
	// Constructors and Destructors
		Computations();
		Computations(const Intersection& inter, Ray *r);

	// setters and getters
		float		getT() const;
		void		setT(float);

		AObject*	getObjPtr() const;
		void		setObjPtr(AObject *);

		const Tuple&	getPoint() const;
		void			setPoint(const Tuple&);

		const Tuple&	getEyev() const;
		void			setEyev(const Tuple&);

		const Tuple&	getNormalv()const;
		void			setNormalv(const Tuple&);
};

std::ostream& operator<<(std::ostream& os, const Computations& comps);

#endif