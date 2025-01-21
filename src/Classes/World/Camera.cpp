#include "Camera.hpp"

/*====== Constructors and Destructors =====*/

Camera::Camera() {

}

Camera::Camera(const Tuple& pov, const Tuple& orient, float f) {
	viewpoint = pov;
	orientation = orient;
	fov = f;
}
