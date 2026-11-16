#include "Camera.hpp"
#include "rt.hpp"
#include "Ray.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*====== Constructors and Destructors =====*/

Camera::Camera() {

}

std::vector<std::string> Camera::requiredKeys() const {
	return {"position", "orientation", "fov"};
}

const char * Camera::typeString() const {
	return "Camera";
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
}

void Camera::updateSize(int actualWidth, int actualHeight)
{
	half_view = (fov * PI) / 180.0;
	float aspect_ratio = (float)actualWidth / (float)actualHeight;
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
	pixel_size = (half_width * 2) / (float)actualWidth;
	transform = Matrix::view_transform(position,
		Tuple::normalize(orientation), Tuple(0, 1, 0, 0));
		inv_transform = transform.invertedMatrix().transpose();
}

void Camera::print() const
{
	std::cout << colorprint::green << "camera\n" << colorprint::reset
	          << "  position:    (" << position.x << ", " << position.y << ", " << position.z << ")\n"
	          << "  orientation: (" << orientation.x << ", " << orientation.y << ", " << orientation.z << ")\n"
	          << "  fov:         " << fov << std::endl;
}

void Camera::rotate(float dYaw, float dPitch)
{
	yaw   += dYaw;
	pitch += dPitch;

	const float limit = 1.55f;                     // about 89 degrees
	pitch = std::max(-limit, std::min(limit, pitch));

	orientation.x = cosf(pitch) * sinf(yaw);
	orientation.y = sinf(pitch);
	orientation.z = cosf(pitch) * cosf(yaw);
	orientation.w = 0.f;

	preBuild();
}

bool Camera::preBuild() {
	half_view = (fov * PI) / 180.0;
	float aspect_ratio = (float)width / (float)height;
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
	pixel_size = (half_width * 2) / (float)width;
	transform = Matrix::view_transform(position,
		Tuple::normalize(orientation), Tuple(0, 1, 0, 0));
		inv_transform = transform.invertedMatrix().transpose();
	if (init == false)
	{
		std::cout << "first" << std::endl;
		yaw = atan2f(orientation.x, orientation.z);
		pitch = asinf(std::clamp(orientation.y, -1.f, 1.f));   
		init = true;
	}
	return true;
}

bool Camera::packIntoGpuScene(GpuScene& s) const
{
	s.cam.inv_transform = inv_transform.getMatrix();
	s.cam.half_width = half_width;
	s.cam.half_height = half_height;
	s.cam.pixel_size = pixel_size;
	s.cam.half_view = half_view;
	return true;
}

bool Camera::setProperty(const std::string& key, const std::string& val)
{
	if (key == "position")    return parsePosition(val, position);
	else if (key == "orientation") return parseVec(val, orientation, 0.f);
	else if (key == "fov")         { try { fov = std::stof(val); } catch (...) { return false; } return true; }
	return false;
}

void Camera::move(float forward, float right, float up)
{
	Tuple f = orientation;
	f.normalize();
	Tuple r(0, 1, 0, 0);
	r = r.cross_product(f);
	r.normalize();
	// normalize(cross_product(f, t_f4{0, 1, 0, 0}));  // flip the order if left/right is inverted
	Tuple u = r.cross_product(f);

	Tuple pos = (f * forward + r * right + u * up) + position;
	position = pos.getValue();
	preBuild();                                              // rebuild the view matrix
}

/*======== SETTERS AND GETTERS ========*/

t_f4 Camera::getPosition(void) const {
	return position;
}
void Camera::setViewPort(int w, int h) {
	width = w;
	height = h;
}