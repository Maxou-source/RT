#pragma once

#include "IParsable.hpp"

class Wobble: public IParsable {
	private:
		float amplitude;
		float frequency;
	public:
		Wobble();
		bool setProperty(const std::string& key, const std::string& val) override;
		bool preBuild() override;
		std::vector<std::string> requiredKeys() const override;
		const char *typeString() const override;
		bool	packIntoGpuScene(GpuScene&) const override;
		void	packInto(GpuMaterial& g) const;
};