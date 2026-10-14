# include "Cylinder.hpp"
//cy -0.5,-5,-33 0,1,0 5 20 255,140,0

Cylinder::Cylinder()
{
}

void Cylinder::printSpecific(std::ostream& os) const
{
	os << "  diameter:   " << diameter << "\n"
	   << "  height:     " << height << "\n";
}

bool Cylinder::setProperty(const std::string& key, const std::string& val)
{
	if (key == "height")
	{
		try { height = std::stof(val); } catch (...) { return false; }
		return true;
	}
	else if (key == "diameter")
	{
		try { diameter = std::stof(val); } catch (...) { return false; }
		return true;
	}
	return AObject::setProperty(key, val);
}