#include "World.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Cube.hpp"
#include "Plane.hpp"
#include "Torus.hpp"
#include "Cone.hpp"
#include "Ray.hpp"
# include <memory>

#define STB_IMAGE_IMPLEMENTATION               // in exactly one .cpp
#include "stb_image.h"
// #define STB_IMAGE_RESIZE_IMPLEMENTATION        // same rule
// #include "stb_image_resize.h"

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
		case ObjectId::CUBE:		objects.push_back(std::make_unique<Cube>()); break;
		case ObjectId::PLANE:		objects.push_back(std::make_unique<Plane>()); break;
		case ObjectId::CONE:		objects.push_back(std::make_unique<Cone>()); break;
		case ObjectId::TORUS:		objects.push_back(std::make_unique<Torus>()); break;
		default: return nullptr;
	}
	return (objects.back().get());
}

Light& World::AddLight()
{
	lights.emplace_back();
	return lights.back();
}

GpuScene World::buildGpuScene()	
{
	GpuScene s{};
	cam.packIntoGpuScene(s);
	amb.packIntoGpuScene(s);
	for (const auto& o : objects)
		if (!o->packIntoGpuScene(s))
			std::cerr << "KO:: too many objects (max " << MAX_OBJECTS << ")" << std::endl;
	for (const auto& l : lights)
		if (!l.packIntoGpuScene(s))
			std::cerr << "KO:: too many lights (max " << MAX_LIGHTS << ")" << std::endl;
	return s;
}

const char* World::objectName(ObjectId id)
{
	static const char* names[] = {"None", "Sphere", "Cylinder", "Cube", "Cone", "AmbientLight"};
	return names[static_cast<int>(id)];
}


void World::printWorld() const
{
	cam.print();
	size_t i = 0;
	for (const auto& obj : objects)
	{
		const Material& m = obj->getMaterial();
		t_f4 pos = obj->getPosition();

		std::cout << "object " << i++ << colorprint::green
		          << " [" << obj->typeString() << "]\n" << colorprint::reset
		          << "  position:   (" << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
		obj->getMaterial().print(std::cout);
		obj->printSpecific(std::cout);   // shape-specific properties
	}
	size_t j = 0;
	for (const auto& light : lights)
		light.print(j++);
	std::cout << lights.size() << " light(s) in world" << std::endl;
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

bool loadImage(const std::string& path, Image& out)
{
	int channels;
	unsigned char* d = stbi_load(path.c_str(), &out.w, &out.h, &channels, 4);  // force RGBA
	if (!d)
	{
		std::cerr << "KO:: cannot load " << path << std::endl;
		return false;
	}
	out.px.assign(d, d + (size_t)out.w * out.h * 4);
	stbi_image_free(d);
	return true;
}

int World::registerTexture(const std::string& path)
{
	auto it = textureIndex.find(path);
	if (it != textureIndex.end())
		return it->second;                     // already loaded: share the layer

	int w, h, channels;
	std::cout << "path" << path << std::endl;
	unsigned char* src = stbi_load(path.c_str(), &w, &h, &channels, 4);   // force RGBA
	if (!src)
	{
		std::cerr << "KO:: cannot load texture " << path << std::endl;
		return -1;
	}

	Image img;
	img.w = w;
	img.h = h;

	img.px.resize(w * h * 4);
	memcpy(img.px.data(), src, w * h * 4);
	
	// assert(img.px.size() == TEX_SIZE * TEX_SIZE * 4);
	textures.push_back(std::move(img));
	int layer = (int)textures.size() - 1;
	textureIndex[path] = layer;
	return layer;
}

bool World::resolveTextures()
{
	for (const auto& obj : objects)
	{
		Material& m = obj->getMaterial();      // non-const accessor
		if (m.hasTexture == false)
			continue;
		std::cout << "objtype or something " << obj->typeString() << std::endl;
		int layer = registerTexture(m.getTexture().file());
		if (layer < 0)
			return false;
		m.getTexture().setLayer(layer);
	}
	return true;
}

float World::sceneScale() const
{
	if (objects.empty())
		return 1.0f;

	t_f4 mn = objects[0]->getPosition();
	t_f4 mx = mn;
	for (const auto& o : objects)
	{
		t_f4 p = o->getPosition();
		mn = {std::min(mn.x, p.x), std::min(mn.y, p.y), std::min(mn.z, p.z), 1};
		mx = {std::max(mx.x, p.x), std::max(mx.y, p.y), std::max(mx.z, p.z), 1};
	}
	t_f4 c = cam.getPosition();                       // include the camera
	mn = {std::min(mn.x, c.x), std::min(mn.y, c.y), std::min(mn.z, c.z), 1};
	mx = {std::max(mx.x, c.x), std::max(mx.y, c.y), std::max(mx.z, c.z), 1};

	t_f4 d = mx - mn;
	float size = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
	return size > 1e-4f ? size : 1.0f;                // degenerate scene: fall back to 1
}

/*================ SETTERS AND GETTERS =========================*/

void World::setCamera(Camera &c) {cam = c;}
Camera& World::getCamera() {return cam;}

void World::setAmbientLight(AmbientLight &c) {amb = c;}
AmbientLight& World::getAmbientLight() {return amb;}
