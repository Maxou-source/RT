#ifndef WORLD_HPP
# define WORLD_HPP

# include <set>
# include <vector>
# include <iostream>
# include <memory>

# include "Intersection.hpp"
# include "Camera.hpp"
# include "rt.hpp"

class AObject;
class Light;
class Ray;

class World {
	private:
		std::set<Intersection>					intersects;
		std::vector<std::unique_ptr<AObject>>	objects;
		std::vector<Light *>					lights;
		Camera									cam;
	public:
	// Constructors and Destructors
		World();

	// method
		AObject*				AddObject(ObjectId id);
		void					add_sphere(float size, float x, float y, float z);
		std::set<Intersection>	intersectWorld(Ray *r);
		void printWorld() const;
		void printIntersection();

		void setCamera(Camera &c);
		Camera& getCamera();
};

#endif