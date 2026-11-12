#pragma once

# include "rt.hpp"
# include "IParsable.hpp"
# include "Pattern.hpp"
# include "Texture.hpp"
# include "Wobble.hpp"


class Material : public IParsable
{
	private:
		t_clr		color     = {1.f, 1.f, 1.f};
		float		ambient   = 1.0f;
		float		diffuse   = 0.9f;
		float		specular  = 0.9f;
		float		shininess = 200.f;
		
		float		reflection = 0.0;
		float transparency = 0;
		float refractive_index = 0.0;

		Pattern			pat;
		Texture			text;
		Wobble			wob;
	public:
		bool			hasTexture = false;
		Material();
		bool setProperty(const std::string& key, const std::string& val) override;
		bool preBuild() override;
		std::vector<std::string> requiredKeys() const override;
		const char *typeString() const override;
		bool packIntoGpuScene(GpuScene&) const override;
		void packInto(GpuMaterial& g) const;
		IParsable* getSubBlock(const std::string& name) override;
		void print(std::ostream& os, const char* indent = "  ") const;

		// getterssss
		Pattern& getPattern() ;
		Texture& getTexture() ;
};