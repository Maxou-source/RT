
#ifndef OBJECT_HPP
# define OBJECT_HPP

# include <iostream>
# include <stdbool.h>
# include "Tuple.hpp"
# include "Matrix.hpp"
# include "rt.hpp"

class Ray;

class AObject
{
	protected:
		static int	idCounter;
		int			id;
		t_f4		center;
		Matrix		m;
		Matrix		i_m;

		t_f4		origin;
		t_clr		color;
		float		diameter;
		float		specular;
		bool		pattern;


	public:
	// Constructors and Destructors
		virtual ~AObject() {}
		AObject();

	// methods
		virtual bool intersect(Ray *r) = 0;

	// setters and getters
		void	setMatrix(Matrix &ma) {m = ma;}
		void	setIMatrix(Matrix &ma) {i_m = ma;}
		Matrix&	getMatrix() {return m;};
		Matrix&	getIMatrix() {return i_m;};
};

#endif