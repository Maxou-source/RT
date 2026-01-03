#include "Camera.hpp"
#include "rt.hpp"
#include "Ray.hpp"

/*====== Constructors and Destructors =====*/

Camera::Camera() {

}

// t_m4	view_transform(t_f4 from, t_f4 forward, t_f4 up)
// {
// 	t_f4	leftv;
// 	t_f4	true_up;
// 	t_m4	res;

// 	leftv = cross_product(forward, normalization(up));
// 	if (equal_tuple(leftv, point(0, 0, 0, 0)))
// 	{
// 		leftv = cross_product(forward, point(0, 0, 1, 0));
// 		if (equal_tuple(leftv, point(0, 0, 0, 0)))
// 			leftv = cross_product(forward, point(1, 0, 0, 0));
// 	}
// 	true_up = cross_product(leftv, forward);
// 	res = build_identity_matrix();
// 	res[0][0] = leftv.x;
// 	res[0][1] = leftv.y;
// 	res[0][2] = leftv.z;
// 	res[1][0] = true_up.x;
// 	res[1][1] = true_up.y;
// 	res[1][2] = true_up.z;
// 	res[2][0] = -forward.x;
// 	res[2][1] = -forward.y;
// 	res[2][2] = -forward.z;
// 	return (res * translated_matrix(-from.x, -from.y, -from.z));
// }

// void	build_camera(t_camera *cam)
// {
// 	half_view = (cam->fov * PI) / 180.0;
// 	cam->aspect_ratio = WIN_WIDTH / WIN_HEIGHT;
// 	if (cam->aspect_ratio >= 1)
// 	{
// 		cam->half_width = cam->half_view;
// 		cam->half_height = cam->half_view / cam->aspect_ratio;
// 	}
// 	else
// 	{
// 		cam->half_width = cam->half_view * cam->aspect_ratio;
// 		cam->half_height = cam->half_view;
// 	}
// 	cam->pixel_size = (cam->half_width * 2) / WIN_WIDTH;
// 	cam->transform = view_transform(cam->viewpoint,
// 			normalization(cam->orientation), point(0, 1, 0, 0));
// 	cam->inv_transform = inverted_matrix(cam->transform);
// }

// void	ray_for_pixel(t_camera *cam, int px, int py, t_ray *r)
// {
// 	float	world_x;
// 	float	world_y;
// 	t_f4	world_point;
// 	t_f4	pixel;

// 	world_x = cam->half_width - (((float)px + 0.5) * cam->pixel_size);
// 	world_y = cam->half_height - (((float)py + 0.5) * cam->pixel_size);
// 	world_point = point(world_x, world_y, -1.0, 1.0);
// 	pixel = matrix_tuple(cam->inv_transform, world_point);
// 	r->origin.w = 1.0;
// 	r->origin = matrix_tuple(cam->inv_transform, point(0, 0, 0, 1.0));
// 	r->direction = normalization(pixel - r->origin);
// 	r->direction.w = 0;
// }

void	Camera::rayForPixel(int px, int py, Ray *r) {
	float	world_x;
	float	world_y;
	Tuple	world_point;
	Tuple	pixel;

	world_x = half_width - (((float)px + 0.5) * pixel_size);
	world_y = half_height - (((float)py + 0.5) * pixel_size);
	world_point = Tuple(world_x, world_y, -1.0, 1.0);
	pixel = world_point * inv_transform;
	// pixel = matrix_tuple(inv_transform, world_point);
	r->setOrigin(Tuple(0,0,0,1.0) * inv_transform);
	// r->origin.w = 1.0;
	// r->origin = matrix_tuple(inv_transform, point(0, 0, 0, 1.0));
	r->setDirection(Tuple::normalize(pixel - r->getOrigin()));
	// r->direction = normalization(pixel - r->origin);
	// r->direction.w = 0;
}

Camera::Camera(const Tuple& pov, const Tuple& orient, float f) {
	half_view = (f * PI) / 180.0;
	float aspect_ratio = 400 / 400;
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
	pixel_size = (half_width * 2) / 400;
	std::cout << "pixel size " << pixel_size << std::endl;
	std::cout << "aspect  " << aspect_ratio << std::endl;
	std::cout << "half_view  " << half_view << std::endl;
	std::cout << "half_width  " << half_width << std::endl;
	std::cout << "half_height " << half_height << std::endl;
	transform = Matrix::view_transform(pov,
			Tuple::normalize(orient), Tuple(0, 1, 0, 0));
	transform.display();
	inv_transform = transform.invertedMatrix();
	// viewpoint = pov;
	// orientation = orient;
	// fov = f;
}


