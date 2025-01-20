#ifndef WORLD_HPP
# define WORLD_HPP

# include <set>
# include <vector>
# include <iostream>

# include "Intersection.hpp"

class AObject;
class Light;
class Ray;

class World {
	private:
		std::set<Intersection> intersects;
		std::vector<AObject *> objects;
		std::vector<Light *> lights;
	public:
	// Constructors and Destructors
		World();

	// method
		void add_sphere(float size, float x, float y, float z);
		std::set<Intersection> intersectWorld(Ray *r);
		void printWorld();
		void printIntersection();

};

#endif