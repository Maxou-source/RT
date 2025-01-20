#ifndef INTERSECTION_HPP
# define INTERSECTION_HPP

# include <set>
# include <ostream>

class AObject;

class Intersection {
	private:
		float		t;
		AObject*	obj;
	public:
	// Constructors and Destructors 
		Intersection();
		Intersection(float t);
		Intersection(float t, AObject *);

	// overload operators
		bool operator<(const Intersection &other) const;

	// getters and setters
		float getT() const;
		void setT(float tmp);

		AObject *getObjPtr() const;
		void 	setObjPtr(AObject *);
};

std::ostream& operator<<(std::ostream& os, const std::set<Intersection>& intersection);

#endif