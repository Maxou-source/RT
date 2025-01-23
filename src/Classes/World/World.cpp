#include "World.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"

/*===== Construcors an Destructors =======*/

World::World() {
	std::cout << "creating world" << std::endl;
	// intersects = 0;
	// objects = 0;
}

/*===== Methods ======*/

void World::add_sphere(float size, float x, float y, float z)
{
	Sphere *sp = new Sphere;
	sp->translate(x, y, z);
	sp->scale(size, size, size);
	sp->applyTransformations();
	objects.push_back(sp);
}

void World::add_object(AObject *g)
{
	objects.push_back(g);
}

void World::printWorld()
{
	std::vector<AObject *>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		std::cout << "object " << std::endl;
	}
}

void World::printIntersection()
{
	std::set<Intersection>::iterator it;
	for (it = intersects.begin(); it != intersects.end(); it++)
	{
		std::cout << "intersected at " << (*it).getT() << std::endl;
	}
}

std::set<Intersection> World::intersectWorld(Ray *r)
{
	std::vector<AObject *>::iterator it;
	std::set<Intersection> result;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		std::set<Intersection> tmp = (*it)->intersect(r);
		if (!tmp.empty())
			result.insert(tmp.begin(), tmp.end());
	}
	return result;
}
// if need more then one intersection just handle it over here
