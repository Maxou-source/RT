#include "Light.hpp"
#include "Sphere.hpp"
#include "ParseUtils.hpp"
#include <math.h>

// using namespace ParseUtils;

Light::Light()
{
	ambient = 0.1;
	diffuse = 0.9;
	specular = 0.9;
	shininess = 200.0;
	color = {1.0f,1.0f,1.0f};
	position = {-10, 10, -10, 0.0f};
}

bool	Light::packIntoGpuScene(GpuScene& s) const {
	std::cout << "packing light" << std::endl;
	if (s.light_count >= MAX_LIGHTS)
		return false;
	GpuLight& l = s.lights[s.light_count++];

	l.color[0] = color.r;
	l.color[1] = color.g;
	l.color[2] = color.b;

	l.position[0] = position[0];
	l.position[1] = position[1];
	l.position[2] = position[2];

	l.ambient = ambient;
	l.specular = specular;
	l.diffuse = diffuse;
	return true;
}

bool	Light::setProperty(const std::string& key, const std::string& val) {
	if (key == "position")
		return parsePosition(val, position);
		try {
			if (key == "color")           return ParseUtils::parseColor(val, color);
			if (key == "ambient")         ambient = std::stof(val);
			else if (key == "diffuse")    diffuse = std::stof(val);
			else if (key == "specular")   specular = std::stof(val);
			else if (key == "shininess")  shininess = std::stof(val);
			else return false;           // not a material key
		} catch (const std::exception&) { return false; }
		return true;
}

bool Light::preBuild() {
	return true;
}

std::vector<std::string> Light::requiredKeys() const {
	return {"position", "ambient", "diffuse", "color"};
}

const char * Light::typeString() const {
	return "Light";
}

void Light::print(size_t index) const
{
	std::cout << "light " << index << colorprint::green << " [point]\n" << colorprint::reset
	          << "  position:  (" << position.x << ", " << position.y << ", " << position.z << ")\n"
	          << "  color:     (" << color.r << ", " << color.g << ", " << color.b << ")\n"
	          << "  shininess: " << shininess << "\n";
}

