#ifndef LIGHT_HPP
# define LIGHT_HPP

# include "AObject.hpp"

class Light {
	private:
		float ambient;
		float diffuse;
		float specular;
		float shininess;
		Tuple color;
		Tuple position;
	public:
		Light();
		Tuple lighting(t_material mat, const Tuple &point,const Tuple &eyev, const Tuple& normalv);
};

#endif