#ifndef RAY_HPP
# define RAY_HPP

# include "Tuple.hpp"

class Ray {
	private:
		Tuple	origin;
		Tuple	direction;
	public:
		// Ray();
	// constructor and Destructors
		Ray(const Tuple&, const Tuple&);
		// Ray(Tuple&, Tuple&);
	
	// methods
		Tuple position(float t);

	// getters and setters
		Tuple& getOrigin();
		Tuple& getDirection();
		void setDirection(Tuple&);
		void setOrigin(Tuple&);
};

#endif