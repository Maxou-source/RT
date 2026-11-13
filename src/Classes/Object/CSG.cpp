#include "CSG.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Cube.hpp"
#include "Plane.hpp"
#include "Torus.hpp"
#include "Cone.hpp"


std::vector<std::string> CSG::requiredKeys() const {
	return {};
}

IParsable* CSG::getSubBlock(const std::string& name) {
	ObjectId id = ParseUtils::findObjectId(name);
	if (id == ObjectId::NONE || children.size() >= 2)
		return nullptr;                            // unknown shape, or a third child
	AObject* child = AddObject(id);   // the same factory as World::AddObject
	if (!child)
		return nullptr;
	return children.back().get();
}

AObject* CSG::AddObject(ObjectId id)
{
	switch (id)
	{
		case ObjectId::SPHERE:		children.push_back(std::make_unique<Sphere>()); break;
		case ObjectId::CYLINDER:	children.push_back(std::make_unique<Cylinder>()); break;
		case ObjectId::CUBE:		children.push_back(std::make_unique<Cube>()); break;
		case ObjectId::PLANE:		children.push_back(std::make_unique<Plane>()); break;
		case ObjectId::CONE:		children.push_back(std::make_unique<Cone>()); break;
		case ObjectId::TORUS:		children.push_back(std::make_unique<Torus>()); break;
		case ObjectId::CSG_UNION:		children.push_back(std::make_unique<CSG>(CSG_UNION)); break;
		case ObjectId::CSG_INTERSECT:		children.push_back(std::make_unique<CSG>(CSG_INTERSECT)); break;
		case ObjectId::CSG_DIFFERENCE:		children.push_back(std::make_unique<CSG>(CSG_DIFFERENCE)); break;
		default: return nullptr;
	}
	return (children.back().get());
}

bool CSG::repeatableSubBlock() const {return true;}

const char * CSG::typeString() const {
	return "CSG";
}

bool CSG::packIntoGpuScene(GpuScene& s) const {
	return true;
}

bool CSG::preBuild() {
	return true;
}

bool CSG::setProperty(const std::string& key, const std::string& val) {
	if (key == "position")    return ParseUtils::parsePosition(val, position);
	if (key == "orientation") return ParseUtils::parseDirection(val, orientation);
	return false;
}