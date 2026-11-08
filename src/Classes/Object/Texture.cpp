#include "Texture.hpp"
#include "ParseUtils.hpp"

Texture::Texture() {
}

bool Texture::setProperty(const std::string& key, const std::string& val) {
	try {
		if (key == "texture_file") {}
		else if (key == "parameter") { param = std::stof(val);}
		else if (key == "orientation") {return ParseUtils::parseVec(val, orientation, 1.0);}
	} catch (const std::exception&) {return false;}
	return true;
}

bool Texture::preBuild()  {
	Matrix scale_Matrix;
	scale_Matrix.scale(param, param, param);
	scale_Matrix.display();

	Matrix orientation_matrix;
	orientation_matrix.orientation(orientation.x, orientation.y,orientation.z);
	orientation_matrix.display();
	i_m = orientation_matrix * scale_Matrix;
	i_m = i_m.invertedMatrix().transpose();
	return true;
}

std::vector<std::string> Texture::requiredKeys() const {
	return {"texture_file", "orientation"};
}

const char *Texture::typeString() const {
	return "Texture";
}

bool Texture::packIntoGpuScene(GpuScene&) const {
	return true;
}

void Texture::packInto(GpuMaterial& g) const {
	std::cout << "packing pattern" << std::endl;
	g.pat.param = param;
	g.pat.i_m = i_m.getMatrix();
	return ;
}

