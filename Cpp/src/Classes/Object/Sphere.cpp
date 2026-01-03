#include "Sphere.hpp"
#include "Ray.hpp"
#include "rt.hpp"

/*==== Constructors and Destructors =====*/

Sphere::Sphere() : AObject() {}

// method
/*
t1 ← (-b - √(discriminant)) / (2 * a)
t2 ← (-b + √(discriminant)) / (2 * a)*/

/*====== Methods =======*/

std::set<Intersection> Sphere::intersect(Ray *r)
{
	Ray newR(r->getOrigin() * i_m, r->getDirection()* i_m);
	Tuple direction = newR.getDirection();
	Tuple sphere_to_ray = newR.getOrigin() - Tuple(0,0,0,POINT);

	float a = direction.dot_product(direction);
	float b = 2 * (direction.dot_product(sphere_to_ray));
	float c = sphere_to_ray.dot_product(sphere_to_ray) - 1;

	float discriminant = (b * b) - (4 * a * c);
	std::set<Intersection> myset;
	if (discriminant < 0)
		return myset;
	Intersection t1(((-b - sqrtf(discriminant)) / (2 * a)), this);
	Intersection t2(((-b + sqrtf(discriminant)) / (2 * a)), this);
	myset.insert(t1);
	myset.insert(t2);
	return myset;
}
/*function normal_at(sphere, p)
return normalize(p - point(0, 0, 0))*/

// t_f4	normal_at(t_object *m, t_f4 p)
// {
// 	t_f4	object_point;
// 	t_f4	object_normal;
// 	t_f4	world_normal;
// 	t_f4	tmp;

// 	object_point = matrix_tuple(m->i_m, p);
// 	object_normal = object_point - point(0, 0, 0, 1);
// 	world_normal = matrix_tuple(m->t_m, object_normal);
// 	tmp = normalization(world_normal);
// 	world_normal.w = 0;
// 	return (tmp);
// }
Tuple	Sphere::normal(const Tuple &t)
{
	Tuple object_normal = (t * i_m) - Tuple(0, 0, 0, POINT);
	Tuple world_normal = object_normal * t_m;
	world_normal.normalize();
	return world_normal;
	// return Tuple::normalize(Tuple(t - Tuple(0,0,0,POINT)));
}