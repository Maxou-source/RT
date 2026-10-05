// #include <string>
#include <iostream>
#include <stdlib.h>
#include "Sphere.hpp"
#include "Ray.hpp"
#include "parse.hpp"
#include "Matrix.hpp"
#include "rt.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include "XCB.hpp"
#include "Light.hpp"
#include "World.hpp"
#include "Intersection.hpp"
#include "Computations.hpp"
#include "Camera.hpp"
#include "VulkanApp.hpp"
// #include <string>

#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>

int	float_to_rgba(t_f4 color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(((color.x > 1.0) + ((color.x < 1.0) * color.x)) * 255.0);
	g = (int)(((color.y > 1.0) + ((color.y < 1.0) * color.y)) * 255.0);
	b = (int)(((color.z > 1.0) + ((color.z < 1.0) * color.z)) * 255.0);
	return ((r << 16) | (g << 8) | b);
}

Tuple color_at(World *w, Ray *r, int px, int py)
{
	std::set<Intersection>	interSet;
	Light l;
	t_material mat;

	interSet = w->intersectWorld(r);
	if (interSet.empty())
		return Tuple(0,0,0,POINT);

	Computations comps(*interSet.begin(), r);
	comps.setPoint(comps.getPoint() + (comps.getNormalv() * EPSILON * 100.0));

	// if (px == 200 && py == 200)
	// {
	// 	std::cout<< comps << std::endl;
	// 	std::cout << "direction " << r->getDirection() << std::endl;
	// }
	mat.color = comps.getObjPtr()->getColor();
	return l.lighting(mat, comps.getPoint(), comps.getEyev(), comps.getNormalv());
}

World buildScene1()
{
	World *w = new World();

	AObject *sp1 = new Sphere();
	Matrix m, m2, m3, m4;
	m.translate(0,0,0);
	// m2.rotation_matrix_y((-PI)/4);
	// m3.rotation_matrix_x(PI/2);
	m4.scale(1, 1, 1);
	sp1->setColor(Tuple(0,1,1, 0));
	sp1->setMatrix(m*m4);
	// sp1->setMatrix(((m2 * m3) * m) * m4);
	// sp1->setMatrix((m * m4) * (m2 * m3));
	// sp1->setMatrix(m4 * m3 * m2 * m);
	sp1->applyTransformations();
	w->add_object(sp1);
	return *w;
}

int main()
{
	VulkanApp vApp;

	vApp.run();

	XCB xcb;
	xcb.setupConnection();
	xcb.setupScreenAndFormat();
	xcb.createWindowAndGC();
	Image img(xcb.getFormatPtr(), WIN_WIDTH, WIN_HEIGHT);
	xcb.setImage(img);

	Ray ray(Tuple(0,0,0, POINT), Tuple(0,0,0,VECTOR));
	Camera cam(Tuple(0,0,-2, POINT), Tuple(0,0,1,VECTOR), 90.0);

	World w = buildScene1();

	// setcameraobject for opengl
	// open .glsl
	while (1)
	{
	
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			for (int x = 0; x < WIN_WIDTH; x++)
			{
			// CAMERA FIXED VALUE half_width half_height ...
				// rayforpixel(cam, x, y, &ray);
				
				// WORLD FIXED VALUE scene basically
				// color at .... 
				cam.rayForPixel(x, y, &ray);
				// if (x == 200 && y == 200)
				// {
				// std::cout << "ray dir = " << ray.getDirection().getValue().x << ", " << ray.getDirection().getValue().y << ", " << ray.getDirection().getValue().z << std::endl;
				// std::cout << "ray pos = " << ray.getOrigin().getValue().x << ", " << ray.getOrigin().getValue().y << ", " << ray.getOrigin().getValue().z << std::endl;
				// }
				img.pixel_put(x,y,float_to_rgba(color_at(&w, &ray, x, y).getValue()));
			
			}
		}


		xcb_put_image(
			xcb.connection,
			XCB_IMAGE_FORMAT_Z_PIXMAP,
			xcb.window,
			xcb.gc,
			xcb.xcb_image.getWidth(), xcb.xcb_image.getHeight(),			// Image dimensions
			0, 0,				// x, y
			0,				   // Left-pad
			xcb.screen->root_depth,  // Depth
			xcb.xcb_image.getTotalSize(),
			xcb.xcb_image.getImageData());
		// need to check return 
		xcb_flush(xcb.connection);
	}
	xcb.loop();
}