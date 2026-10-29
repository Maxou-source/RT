#ifndef LIGHT_HPP
# define LIGHT_HPP

# include "AObject.hpp"

class Light: public IParsable {
	private:
		float	ambient;
		float	diffuse;
		float	specular;
		float	shininess;
		t_clr	color;
		t_f4	position;
	public:
		Light();
		bool						packIntoGpuScene(GpuScene& s) const override;
		bool						setProperty(const std::string& key, const std::string& val) override;
		bool						preBuild() override;
		std::vector<std::string>	requiredKeys() const override;
		const char *				typeString() const override;
		void						print(size_t index) const;
};

#endif