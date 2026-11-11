#pragma once

# include <istream>

#include "IParsable.hpp"


struct Image { int w = 0, h = 0; std::vector<unsigned char> px; };


class Texture: public IParsable {
	private:
		std::string	texture_file;
		t_f4		orientation = {0, 1, 0, 0};
		float		param       = 1.f;
		int			layer       = -1;
		Matrix		i_m;
	public:
		Texture();
		bool setProperty(const std::string& key, const std::string& val) override;
		bool preBuild() override;
		std::vector<std::string> requiredKeys() const override;
		const char *typeString() const override;
		bool	packIntoGpuScene(GpuScene&) const override;
		void	packInto(GpuMaterial& g) const;
		std::string& file() {return texture_file;}
		void setLayer(int l) {layer = l;}
};