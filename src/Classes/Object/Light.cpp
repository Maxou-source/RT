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
	source = {-10, 10, -10, 0.0f};
}

bool	Light::packIntoGpuScene(GpuScene& s) const {
	std::cout << "packing light" << std::endl;
	if (s.light_count >= MAX_LIGHTS)
		return false;
	GpuLight& l = s.lights[s.light_count++];

	l.color[0] = color.r;
	l.color[1] = color.g;
	l.color[2] = color.b;

	l.source[0] = source[0];
	l.source[1] = source[1];
	l.source[2] = source[2];

	l.ambient = ambient;
	l.specular = specular;
	l.diffuse = diffuse;
	l.type = type;
	return true;
}

bool	Light::setProperty(const std::string& key, const std::string& val) {
	if (key == "source")
		return parsePosition(val, source);
		try {
			if (key == "color")           return ParseUtils::parseColor(val, color);
			else if (key == "ambient")         ambient = std::stof(val);
			else if (key == "diffuse")    diffuse = std::stof(val);
			else if (key == "specular")   specular = std::stof(val);
			else if (key == "shininess")  shininess = std::stof(val);
			else if (key == "type")			return (parseLightType(val));
			else return false;           // not a material key
		} catch (const std::exception&) { return false; }
		return true;
}

bool Light::preBuild() {
	return true;
}

std::vector<std::string> Light::requiredKeys() const {
	return {"source", "ambient", "diffuse", "color", "type"};
}

const char * Light::typeString() const {
	return "Light";
}

void Light::print(size_t index) const
{
	std::cout << "light " << index << colorprint::green << " [point]\n" << colorprint::reset
	          << "  position:  (" << source.x << ", " << source.y << ", " << source.z << ")\n"
	          << "  color:     (" << color.r << ", " << color.g << ", " << color.b << ")\n"
	          << "  shininess: " << shininess << "\n";
}

bool Light::parseLightType(const std::string& value) {
	std::unordered_map<std::string, LightType> map = {
		{"directional", LightType::DIRECTIONAL}, {"positional", LightType::POSITIONAL},
	};
	auto it = map.find(value);
	if (it == map.end())
		return false;
	type = it->second;
	return true;
}

bool Light::repeatableSubBlock() const {return false;}