#include "World.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Ray.hpp"
# include <memory>

/*===== Construcors an Destructors =======*/

World::World() {
	std::cout << "creating world" << std::endl;
	// intersects = 0;
	// objects = 0;
}

/*===== Methods ======*/

AObject* World::AddObject(ObjectId id)
{
	switch (id)
	{
		case ObjectId::SPHERE:		objects.push_back(std::make_unique<Sphere>()); break;
		case ObjectId::CYLINDER:	objects.push_back(std::make_unique<Cylinder>()); break;
		default: return nullptr;
	}
	return (objects.back().get());
}

GpuScene World::buildGpuScene()
{
	GpuScene s{};
	cam.packIntoGpuScene(s);
	// for (const auto& o : objects)
	// 	if (!o.PackIntoGpuScene(s))
			// std::cerr << "KO:: too many objects (max " << MAX_OBJECTS << ")" << std::endl;
	// for (const auto& l : lights)
	// 	if (!l.PackIntoGpuScene(s))
	// 		std::cerr << "KO:: too many lights (max " << MAX_LIGHTS << ")" << std::endl;
	return s;
}



const char* World::objectName(ObjectId id)
{
	static const char* names[] = {"None", "Sphere", "Cylinder"};
	return names[static_cast<int>(id)];
}

void World::printWorld() const
{
	cam.print();                       // camera first
	size_t i = 0;
	for (const auto& obj : objects)
	{
		const Material& m = obj->getMaterial();
		t_f4 pos = obj->getPosition();

		std::cout << "object " << i++ << colorprint::green
		          << " [" << objectName(obj->type()) << "]\n" << colorprint::reset
		          << "  position:   (" << pos.x << ", " << pos.y << ", " << pos.z << ")\n"
		          << "  color:      (" << m.color.r << ", " << m.color.g << ", " << m.color.b << ")\n"
		          << "  ambient:    " << m.ambient << "\n"
		          << "  diffuse:    " << m.diffuse << "\n"
		          << "  specular:   " << m.specular << "\n"
		          << "  shininess:  " << m.shininess << "\n"
		          << "  reflective: " << m.reflective << "\n"
		          << "  pattern:    " << (m.pattern ? "true" : "false") << "\n"
		          << "  texture:    " << (m.texture.empty() ? "none" : m.texture) << "\n";
		obj->printSpecific(std::cout);   // shape-specific properties
	}
	std::cout << objects.size() << " object(s) in world" << std::endl;
}
void World::printIntersection()
{
	std::set<Intersection>::iterator it;
	for (it = intersects.begin(); it != intersects.end(); it++)
	{
		std::cout << "intersected at " << (*it).getT() << std::endl;
	}
}

/*================ SETTERS AND GETTERS =========================*/

void World::setCamera(Camera &c) {cam = c;}
Camera& World::getCamera() {return cam;}
