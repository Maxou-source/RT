#include "Sphere.hpp"
#include "Ray.hpp"

Sphere::Sphere() : AObject() {}

// method

bool Sphere::intersect(Ray *r)
{
	Tuple direction = r->getDirection();
	direction.display();
	Tuple sphere_to_ray = r->getOrigin() - Tuple(0,0,0,1);
	// need to check if its okqy to have a vector here w = 1
	float a = direction.dot_product(direction);
	float b = 2 * (direction.dot_product(sphere_to_ray));
	float c = sphere_to_ray.dot_product(sphere_to_ray) - 1;
	// sphere_to_ray.display();
	std::cout << "a " << a << std::endl;
	// std::cout << "b " << b << std::endl;
	// std::cout << "c " << c << std::endl;
	float discriminant = (b * b) - (4 * a * c);
	std::cout << "discriminant " << discriminant << std::endl;
	if (discriminant < 0)
	{

		return false;
	}
	return true;
}