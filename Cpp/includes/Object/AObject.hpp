
#ifndef OBJECT_HPP
# define OBJECT_HPP

# include <iostream>
# include <stdbool.h>
# include <set>
# include "Intersection.hpp"
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
		Matrix		t_m;

		int			color_hex;
		t_material	mat;
		// on va stocker l'image directement ici Jean Marc

		Tuple		origin;
		Tuple		normalv;
		Tuple		reflectv;
		t_clr		color;
		float		diameter;
		float		specular;
		bool		pattern;


	public:
	// Constructors and Destructors
		virtual ~AObject() {}
		AObject();

	// methods
		virtual std::set<Intersection> intersect(Ray *r) = 0;
		virtual Tuple normal(const Tuple &) = 0;
		
		void	reflect(const Tuple&);
		void	scale(float x, float y, float z);
		void	translate(float x, float y, float z);

		void	applyTransformations();

	// setters and getters
		void	setMatrix(const Matrix &ma);
		void	setIMatrix(Matrix &ma);
		Matrix&	getMatrix();
		Matrix&	getIMatrix();
		// Tuple getColor() const;
		// void setColor(const Tuple&);

		void setColor(const t_clr& c);
		void setDiameter(const float d);
		void setSpecular(const float s);
		void setPattern(const bool b);

		t_clr getColor() const;
		float getDiameter() const;
		float getSpecular() const;
		bool  getPattern() const;
};

#endif