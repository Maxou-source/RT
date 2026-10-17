#include "Sphere.hpp"
#include "Ray.hpp"
#include "rt.hpp"

/*==== Constructors and Destructors =====*/

Sphere::Sphere() : AObject() {
}

// Sphere
void Sphere::printSpecific(std::ostream& os) const
{
	os << "  diameter:   " << diameter << "\n";
}

bool Sphere::setProperty(const std::string& key, const std::string& val)
{
	if (key == "diameter")
	{
		try { diameter = std::stof(val); } catch (...) { return false; }
		return true;
	}
	return AObject::setProperty(key, val);
}

std::vector<std::string> Sphere::requiredKeys()  const {
	return ParseUtils::concat(AObject::requiredKeys(), {"diameter"});
}

const char * Sphere::typeString() const {
	return "Sphere";
}

bool Sphere::preBuild() {
	return true;
}

bool Sphere::packIntoGpuScene(GpuScene& s) {
	return true;
}
