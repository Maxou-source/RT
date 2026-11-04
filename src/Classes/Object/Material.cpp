#include "Material.hpp"
#include "ParseUtils.hpp"

Material::Material() {

}

bool Material::setProperty(const std::string& key, const std::string& val) {
	try {
		if (key == "color")           return ParseUtils::parseColor(val, color);
		else if (key == "ambient")         ambient = std::stof(val);
		else if (key == "diffuse")    diffuse = std::stof(val);
		else if (key == "specular")   specular = std::stof(val);
		else if (key == "shininess")  shininess = std::stof(val);
		else return false;           // not a material key
	} catch (const std::exception&) { return false; }
	return true;
}

bool Material::preBuild() {
	return true;
}

std::vector<std::string> Material::requiredKeys() const {
	return {"color"};
}

const char *Material::typeString() const {return "Material";}

bool Material::packIntoGpuScene(GpuScene&) const {
	return true;
}

void Material::packInto(GpuMaterial& g) const
{
	g.color[0]   = color.r;
	g.color[1]   = color.g;
	g.color[2]   = color.b;
	g.color[3]   = 1.0f;
	g.ambient    = ambient;
	g.diffuse    = diffuse;
	g.specular   = specular;
	g.shininess  = shininess;
	g.texture    = -1;
}

IParsable* Material::getSubBlock(const std::string& name) {
	if (name == "Pattern")
		return &pat;
	return nullptr;
}


void Material::print(std::ostream& os, const char* indent) const
{
	os << indent << "material\n"
	   << indent << "  color:      (" << color.r << ", " << color.g << ", " << color.b << ")\n"
	   << indent << "  ambient:    " << ambient    << "\n"
	   << indent << "  diffuse:    " << diffuse    << "\n"
	   << indent << "  specular:   " << specular   << "\n"
	   << indent << "  shininess:  " << shininess  << "\n";
	if (pat.getType() == NO_PATTERN)
		return ;
	else if (pat.getType() == CHECKER)
		os << indent << "Checker \n";
	else
		os << indent << "Stripe \n"; 
}