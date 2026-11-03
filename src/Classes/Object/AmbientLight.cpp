#include "AmbientLight.hpp"
#include "ParseUtils.hpp"


AmbientLight::AmbientLight()
{
}

bool	AmbientLight::packIntoGpuScene(GpuScene& s) const {

	s.ambient.colorAndIntensity[0] = color.r;
	s.ambient.colorAndIntensity[1] = color.g;
	s.ambient.colorAndIntensity[2] = color.b;
	s.ambient.colorAndIntensity[3] = intensity;
	return true;
}

bool	AmbientLight::setProperty(const std::string& key, const std::string& val) {
	try {
			if (key == "color")           return ParseUtils::parseColor(val, color);
			else if (key == "intensity") intensity = std::stof(val);
	} catch (const std::exception&) { return false; }
		return true;
}

bool AmbientLight::preBuild() {
	return true;
}

std::vector<std::string> AmbientLight::requiredKeys() const {
	return {"intensity", "color"};
}

const char * AmbientLight::typeString() const {
	return "AmbientLight";
}

void AmbientLight::print(size_t index) const
{
	std::cout << "Ambientlight " << colorprint::green << " [point]\n" << colorprint::reset
	          << "  color:     (" << color.r << ", " << color.g << ", " << color.b << ")\n"
	          << "  intensity: " << intensity << "\n";
}