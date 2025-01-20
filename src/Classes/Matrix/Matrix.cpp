#include "Matrix.hpp"
#include "rt.hpp"
/*====== Constructors and Destructors ======*/

Matrix::Matrix() {
	int		i;
	int		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			if (i == j)
				matrix[i][j] = 1;
			else
				matrix[i][j] = 0;
		}
	}
	// by default i am building the identity matrix
}

Matrix::Matrix(t_m4 m) {matrix = m;}

Matrix::Matrix(t_f4 a, t_f4 b, t_f4 c, t_f4 d) 
{
	matrix[0][0] = a.x;
	matrix[1][0] = a.y;
	matrix[2][0] = a.z;
	matrix[3][0] = a.w;
	matrix[0][1] = b.x;
	matrix[1][1] = b.y;
	matrix[2][1] = b.z;
	matrix[3][1] = b.w;
	matrix[0][2] = c.x;
	matrix[1][2] = c.y;
	matrix[2][2] = c.z;
	matrix[3][2] = c.w;
	matrix[0][3] = d.x;
	matrix[1][3] = d.y;
	matrix[2][3] = d.z;
	matrix[3][3] = d.w;
}

/*======= Overload Operators =====*/

Matrix Matrix::operator*(const Matrix &m)
{
	Matrix res;
	res.matrix = m.matrix * this->matrix;
	return res;
}

/*====== Methods =======*/

void Matrix::scale(float x, float y, float z)
{
	matrix[0][0] = x;
	matrix[1][1] = y;
	matrix[2][2] = z;
}

void Matrix::translate(float x, float y, float z)
{
	matrix[0][3] = x;
	matrix[1][3] = y;
	matrix[2][3] = z;
}

Matrix	Matrix::invertedMatrix()
{
	t_m4	cof_m;
	float	cof_tmp;
	float	det;
	int		i;
	int		j;

	det = determinant();
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			cof_tmp = cofactor4(i, j);
			cof_m[j][i] = cof_tmp / det;
			j++;
		}
		i++;
	}
	Matrix res(cof_m);
	return (res);
}

Matrix	Matrix::transpose()
{
	t_m4	res;
	int		i;
	int		j;

	i = 0;
	while (i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			res[j][i] = this->matrix[i][j];
		}
		i++;
	}
	Matrix tmp(res);
	return (tmp);
}

void Matrix::display()
{
	int	i;
	int	j;

	i = 0;
	printf("matrixxxxx\n");
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			printf("%f ", matrix[i][j]);
			j++;
		}
		i++;
		printf("\n");
	}
}

/*==Utils==*/

t_m3	Matrix::submatrix(int row, int column)
{
	t_m3	res;
	int		ii;
	int		jj;
	int		i;
	int		j;

	ii = 0;
	jj = 0;
	i = -1;
	while (++i < 4)
	{
		if (i != row)
		{
			jj = 0;
			j = -1;
			while (++j < 4)
			{
				if (j != column)
					res[ii][jj++] = matrix[i][j];
			}
			ii++;
		}
	}
	return (res);
}

float Matrix::determinant()
{
	float	res;
	float	cof;
	t_m3	sub3;
	int		i;

	res = 0;
	i = -1;
	while (++i < 4)
	{
		sub3 = submatrix(0, i);
		cof = sub3[0][0] * (sub3[1][1] * sub3[2][2] - sub3[1][2] * sub3[2][1]) 
			- sub3[0][1] * (sub3[1][0] * sub3[2][2] - sub3[1][2] * sub3[2][0])
			+ sub3[0][2] * (sub3[1][0] * sub3[2][1] - sub3[1][1] * sub3[2][0]);
		if (i % 2)
			cof = cof * -1;
		res += (matrix[0][i] * cof);
	}
	return (res);
}

float Matrix::cofactor4(int row, int column)
{
	float	res;
	t_m3	sub;
	int		i;

	sub = submatrix(row, column);
	res = 0;
	i = -1;
	while (++i < 3)
		res += cofactor3(sub, 0, i) * sub[0][i];
	if (((row + column) % 2) != 0)
		return (res * -1); 
	return (res);
}

t_m2	Matrix::submatrix_f3(t_m3 matrix, int row, int column)
{
	t_m2	res;
	int		ii;
	int		jj;
	int		i;
	int		j;

	ii = 0;
	jj = 0;
	i = -1;
	while (++i < 3)
	{
		if (i != row)
		{
			jj = 0;
			j = -1;
			while (++j < 3)
			{
				if (j != column)
					res[ii][jj++] = matrix[i][j];
			}
			ii++;
		}
	}
	return (res);
}

float	Matrix::determinant_2b2(t_m2 m)
{
	return ((m[0][0] * m[1][1]) - (m[0][1] * m[1][0]));
}

float	Matrix::minor_f3(t_m3 m, int row, int column)
{
	return (determinant_2b2(submatrix_f3(m, row, column)));
}

float	Matrix::cofactor3(t_m3 m, int row, int column)
{
	float	res;

	if (((row + column) % 2) != 0)
	{
		res = minor_f3(m, row, column) * -1;
		return (res);
	}
	return (minor_f3(m, row, column));
}