#pragma once

# include "rt.hpp"
# include "IParsable.hpp"


class Material : public IParsable
{
	private:
		t_clr		color     = {1.f, 1.f, 1.f};
		float		ambient   = 0.1f;
		float		diffuse   = 0.9f;
		float		specular  = 0.9f;
		float		shininess = 200.f;
		float		reflective = 0.f;
		bool		pattern   = false;
		std::string texture;               // empty = none

	public:
		Material();
		bool setProperty(const std::string& key, const std::string& val) override;
		bool preBuild() override;
		std::vector<std::string> requiredKeys() const override;
		const char *typeString() const override;
		bool packIntoGpuScene(GpuScene&) const override;
		void packInto(GpuMaterial& g) const;
		void print(std::ostream& os, const char* indent = "  ") const;

};