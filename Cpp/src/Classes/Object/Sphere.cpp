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
	if (t1.getT() > 0)
		myset.insert(t1);
	if (t2.getT() > 0)
		myset.insert(t2);
	return myset;
}

Tuple	Sphere::normal(const Tuple &t)
{
	// Tuple object_normal = (t * i_m) - Tuple(0, 0, 0, POINT);
	// Tuple world_normal = object_normal * t_m;
	// world_normal.normalize();
	// return world_normal;
	return Tuple::normalize(Tuple(t - Tuple(0,0,0,POINT)));
}