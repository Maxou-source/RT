
#ifndef OBJECT_HPP
# define OBJECT_HPP

# include <iostream>
# include <stdbool.h>
# include "Tuple.hpp"
# include "rt.hpp"

class Ray;

class AObject
{
	protected:
		static int	idCounter;
		int			id;
		t_f4		center;

		t_f4		origin;
		t_clr		color;
		float		diameter;
		float		specular;
		bool		pattern;


	public:
		virtual ~AObject() {}
		AObject();

		virtual bool intersect(Ray *r) = 0;
};

#endif