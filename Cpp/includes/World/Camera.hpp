#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "Tuple.hpp"
# include "Matrix.hpp"

class Ray;

class Camera {
	private:
		Tuple	viewpoint;
		Tuple	orientation;

		// float	fov;

		float	half_width;
		float	half_height;
		float	half_view;
		float	pixel_size;

		Matrix	transform;
		Matrix	inv_transform;
	public:
	
	// Constructors and Destructors 
		Camera();
		Camera(const Tuple&, const Tuple&, float );

	// Methods
		void	rayForPixel(int px, int py, Ray *r);

		void	setOpenGlObject();

};

#endif