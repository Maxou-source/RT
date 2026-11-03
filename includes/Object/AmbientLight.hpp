#pragma once

# include "IParsable.hpp"

class AmbientLight: public IParsable {
	private:
		float	intensity = 0.2;
		t_clr	color {1.0,1.0,1.0};
	public:
		AmbientLight();
		bool						packIntoGpuScene(GpuScene& s) const override;
		bool						setProperty(const std::string& key, const std::string& val) override;
		bool						preBuild() override;
		std::vector<std::string>	requiredKeys() const override;
		const char *				typeString() const override;
		void						print(size_t index) const;
};