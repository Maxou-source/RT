#pragma once

#include "IParsable.hpp"

class Texture: public IParsable {
	private:
		std::string	texture_file;
		Matrix		i_m;
		int			param;
		t_f4		orientation;
	public:
		Texture();
		bool setProperty(const std::string& key, const std::string& val) override;
		bool preBuild() override;
		std::vector<std::string> requiredKeys() const override;
		const char *typeString() const override;
		bool packIntoGpuScene(GpuScene&) const override;
		void packInto(GpuMaterial& g) const;

		bool parsePatternType(const std::string& vec);
};