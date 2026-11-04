#pragma once

#include "IParsable.hpp"
#include <unordered_map>

enum PatternType {
	NO_PATTERN,
	STRIPE,
	CHECKER,
	GRADIENT,
	RING,
	PERLIN
};

class Pattern: public IParsable {
	private:
		PatternType type = NO_PATTERN;
		float param;
		t_clr clr;
	public :
		Pattern();
		bool setProperty(const std::string& key, const std::string& val) override;
		bool preBuild() override;
		std::vector<std::string> requiredKeys() const override;
		const char *typeString() const override;
		bool packIntoGpuScene(GpuScene&) const override;
		void packInto(GpuMaterial& g) const;

		bool parsePatternType(const std::string& vec);

		// getters and setters 	
		PatternType getType() const;
};