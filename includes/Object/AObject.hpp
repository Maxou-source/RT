
#ifndef OBJECT_HPP
# define OBJECT_HPP

# include <iostream>
# include <stdbool.h>
# include "Tuple.hpp"
# include "Matrix.hpp"
# include "rt.hpp"

class Ray;

typedef struct s_material {
	Tuple color;
} t_material;

class AObject
{
	protected:
		static int	idCounter;
		int			id;
		t_f4		center;

		Matrix		m;
		Matrix		i_m;

		Tuple		color;
		int			color_hex;
		// on va stocker l'image directement ici Jean Marc

		Tuple		origin;
		Tuple		normalv;
		Tuple		reflectv;
		// t_clr		color;
		// float		diameter;
		// float		specular;
		// bool		pattern;


	public:
	// Constructors and Destructors
		virtual ~AObject() {}
		AObject();

	// methods
		virtual float intersect(Ray *r) = 0;
		virtual Tuple normal(const Tuple &) = 0;
		
		void reflect(const Tuple&);
		void	scale(float x, float y, float z);

	// setters and getters
		void	setMatrix(Matrix &ma);
		void	setIMatrix(Matrix &ma);
		Matrix&	getMatrix();
		Matrix&	getIMatrix();
};

#endif