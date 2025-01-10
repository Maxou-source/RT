#ifndef MATRIX_HPP
# define MATRIX_HPP

# include "rt.hpp"

typedef float	t_m4 __attribute__((matrix_type(4, 4)));
typedef float	t_m3 __attribute__((matrix_type(3, 3)));
typedef float	t_m2 __attribute__((matrix_type(2, 2)));

class Matrix {
	private:
		t_m4	matrix;
	public:
	// Constructors and Destructors
		Matrix();
		Matrix(t_m4 m);
		Matrix(t_f4 a, t_f4 b, t_f4 c, t_f4 d);

	// methods
		void	scale(float x, float y, float z);
		Matrix	invertedMatrix();

		void	display();
		
	// utils
		t_m3	submatrix(int row, int column);
		float	determinant();
		float	cofactor4(int row, int column);
		float	cofactor3(t_m3 m, int row, int column);

	// setters and getters
		t_m4	getMatrix() {return matrix;}
};

#endif