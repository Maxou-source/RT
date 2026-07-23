#include "Light.hpp"
#include "Sphere.hpp"
#include <math.h>

/*====== Constructors and Destructors =====*/

Light::Light()
{
	ambient = 0.1;
	diffuse = 0.9;
	specular = 0.9;
	shininess = 200.0;
	color = Tuple(1,1,1,POINT);
	position = Tuple(-10, 10, -10, POINT);
	// VALEUR EN DURE A CHANGER
}


/*===== Methods =====*/

Tuple Light::lighting(t_material mat, const Tuple& point, const Tuple& eyev, const Tuple& normalv)
{
	Tuple eff_color = mat.color * color;
	Tuple lightv = Tuple::normalize(position - point);
	Tuple new_ambient = eff_color * ambient;
	float light_dot_normal = normalv.dot_product(lightv);
	if (light_dot_normal < 0)
	{
		return new_ambient;
	}
	else 
	{
		Tuple diffuset = eff_color * 0.9 * light_dot_normal;
		Tuple reflectv = Tuple::reflect(Tuple::negating(lightv), normalv);
		float reflect_dot_eye = reflectv.dot_product(eyev);
		if (reflect_dot_eye < 0)
		{
			Tuple new_specular = Tuple(0,0,0,POINT);
			return new_ambient + diffuset + new_specular;
		}
		float factor = powf(reflect_dot_eye, shininess);
		Tuple specularv = color * specular * factor;
		return new_ambient + diffuset + specularv;
	}
}