#include "Camera.hpp"
#include "rt.hpp"
#include "TupleUtils.hpp"
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
		normalize(orientation), point(0, 1, 0, 0));
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
		normalize(orientation), point(0, 1, 0, 0));
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
	if (key == "position")
		return parsePosition(val, position);
	else if (key == "orientation")
		return parseVec(val, orientation, 0.f);
	else if (key == "fov") {
		try {
			fov = std::stof(val);
		} catch (...) { return false; }
		return true;
	}
	return false;
}

void Camera::move(float forward, float right, float up)
{
	t_f4 f = orientation;
	f = normalize(f);
	t_f4 r = point(0, 1, 0, 0);
	r = cross_product(r, f);
	r = normalize(r);
	t_f4 u = cross_product(r, f);

	t_f4 pos = (f * forward + r * right + u * up) + position;
	position = pos;
	preBuild();
}

/*======== SETTERS AND GETTERS ========*/

t_f4 Camera::getPosition(void) const {
	return position;
}
void Camera::setViewPort(int w, int h) {
	width = w;
	height = h;
}