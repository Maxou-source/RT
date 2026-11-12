#include "Wobble.hpp"

Wobble::Wobble() {

}

bool Wobble::setProperty(const std::string& key, const std::string& val) {
	try {
		if (key == "amplitude") {
			amplitude = std::stof(val); return true;
		}
		else if (key == "frequency") { frequency = std::stof(val); return true;}
	}
	catch (const std::exception&) {return false;}
	return false;
}

bool Wobble::preBuild() {
	return true;
}

std::vector<std::string> Wobble::requiredKeys() const {
	return {"amplitude", "frequency"};
}

const char* Wobble::typeString() const {
	return "Wobble";
}

bool Wobble::packIntoGpuScene(GpuScene& g) const {
	return true;
}

void Wobble::packInto(GpuMaterial& g) const {
	g.wob.amplitude = amplitude;
	g.wob.frequency = frequency;
}
