#include "TupleUtils.hpp"

bool	equal(float a, float b)
{
	if (fabs(a - b) < EPSILON)
		return (true);
	return (false);
}

bool	equal_tuple(t_f4 a, t_f4 b)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (!equal(a[i], b[i]))
			return (false);
		i++;
	}
	return (true);
}

float	magnitude(t_f4 a)
{
	int		i;
	float	res;

	i = 0;
	res = 0;
	while (i < 3)
	{
		res += a[i] * a[i];
		i++;
	}
	return (sqrtf(res));
}

t_f4	normalize(t_f4 a)
{
	float	b;
	t_f4	res;
	int		i;

	res = 0;
	b = magnitude(a);
	if (equal(b, 0))
		return (res);
	i = 0;
	while (i < 3)
	{
		res[i] = a[i] / b;
		i++;
	}
	return (res);
}

t_f4	cross_product(t_f4 a, t_f4 b)
{
	t_f4	res;

	res.x = a.y * b.z - a.z * b.y;
	res.y = a.z * b.x - a.x * b.z;
	res.z = a.x * b.y - a.y * b.x;
	res.w = 0;
	return (res);
}

t_f4	point(float x, float y, float z, float vector_point)
{
	t_f4	res;

	res.x = x;
	res.y = y;
	res.z = z;
	res.w = vector_point;
	return (res);
}
