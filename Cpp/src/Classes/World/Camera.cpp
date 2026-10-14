#include "Camera.hpp"
#include "rt.hpp"
#include "Ray.hpp"
#include <glad/glad.h>   // must be included BEFORE glfw3.h
#include <GLFW/glfw3.h>

/*====== Constructors and Destructors =====*/

Camera::Camera() {

}

void	Camera::rayForPixel(int px, int py, Ray *r) {
	float	world_x;
	float	world_y;
	Tuple	world_point;
	Tuple	pixel;

	world_x = half_width - (((float)px + 0.5) * pixel_size);
	world_y = half_height - (((float)py + 0.5) * pixel_size);
	world_point = Tuple(world_x, world_y, -1.0, 1.0);
	pixel = world_point * inv_transform;
	r->setOrigin(Tuple(0,0,0,1.0) * inv_transform);
	r->setDirection(Tuple::normalize(pixel - r->getOrigin()));
	if (px == 200 && px == 200)
	{

	}
}

Camera::Camera(unsigned int shaderProgram, const Tuple& pov, const Tuple& orient, float f) {
	half_view = (f * PI) / 180.0;
	float aspect_ratio = (float)WIN_WIDTH / (float)WIN_HEIGHT;
	std::cout << "aspect _ratio "  << aspect_ratio << std::endl;
	if (aspect_ratio >= 1)
	{
		half_width = half_view;
		half_height = half_view / aspect_ratio;
	}
	else
	{
		half_width = half_view * aspect_ratio;
		half_height = half_view;
	}
	pixel_size = (half_width * 2) / (float)WIN_WIDTH;
	std::cout << "(FROM CPU) half_view = " << half_width << std::endl;
	std::cout << "(FROM CPU) pixel_size = " << pixel_size << std::endl;

	transform = Matrix::view_transform(pov,
			Tuple::normalize(orient), Tuple(0, 1, 0, 0));

	inv_transform = transform.invertedMatrix();

	glUniform1f(glGetUniformLocation(shaderProgram, "cam.half_view"), half_view);
	glUniform1f(glGetUniformLocation(shaderProgram, "cam.half_height"), half_height);
	glUniform1f(glGetUniformLocation(shaderProgram, "cam.half_width"), half_width);
	glUniform1f(glGetUniformLocation(shaderProgram, "cam.pixel_size"), pixel_size);

	auto matmat = transform.getMatrix();
	auto invmat = inv_transform.getMatrix();
	std::cout << colorprint::green << "(FROM CPU) pov : " << colorprint::reset << pov << std::endl;
	std::cout << colorprint::green << "(FROM CPU) orientation : " << colorprint::reset << orient << std::endl;
	transform.display();
	inv_transform.display();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "cam.transform"),1, GL_TRUE, (float *)(&matmat) );
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "cam.inv_transform"),1, GL_TRUE, (float *)(&invmat) );
}

Camera::Camera(const Tuple& pov, const Tuple& orient, float f) {
	half_view = (f * PI) / 180.0;
	float aspect_ratio = WIN_WIDTH / WIN_HEIGHT;
	if (aspect_ratio >= 1)
	{
		half_width = half_view;
		half_height = half_view / aspect_ratio;
	}
	else
	{
		half_width = half_view * aspect_ratio;
		half_height = half_view;
	}
	pixel_size = (half_width * 2) / WIN_WIDTH;
	std::cout << colorprint::green << "CAMERA (CPU)" << colorprint::reset << std::flush;
	std::cout << " half_width: " << half_width << std::flush;
	std::cout << " half_height: " << half_height << std::flush;
	std::cout << " half_view: " << half_view << std::flush;
	std::cout << " pixel_size: " << pixel_size << std::endl;
	transform = Matrix::view_transform(pov,
			Tuple::normalize(orient), Tuple(0, 1, 0, 0));
	inv_transform = transform.invertedMatrix();
	transform.display();
	inv_transform.display();
}

bool Camera::buildMatrix() {
	return true;
}

bool Camera::setProperty(const std::string& key, const std::string& val)
{
	if (key == "position")    return parsePosition(val, position);
	if (key == "orientation") return parseVec(val, orientation, 0.f);
	if (key == "fov")         { try { fov = std::stof(val); } catch (...) { return false; } return true; }
	return false;
}
