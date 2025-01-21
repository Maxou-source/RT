#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "Tuple.hpp"

class Camera {
	private:
		Tuple	viewpoint;
		Tuple	orientation;

		float	fov;
	public:
		Camera();
		Camera(const Tuple&, const Tuple&, float );


};

#endif