#pragma once

# include "IParsable.hpp"

class AmbientLight: public IParsable {
	private:
		float	intensity;
		t_clr	color;
	public:
		AmbientLight();
		bool						packIntoGpuScene(GpuScene& s) const override;
		bool						setProperty(const std::string& key, const std::string& val) override;
		bool						preBuild() override;
		std::vector<std::string>	requiredKeys() const override;
		const char *				typeString() const override;
		void						print(size_t index) const;
};