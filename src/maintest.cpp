#include <string>
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

int main()
{
	/*====== GRAPHICAL TESTS======*/
	// setting up graphic stuff
	XCB xcb;
	xcb.setupConnection();
	xcb.setupScreenAndFormat();
	xcb.createWindowAndGC();
	Image img(xcb.getFormatPtr(), 400, 400);
	xcb.setImage(img);

	// building image
	unsigned int red = 0xFFFF0000;

	Tuple origin(0,0,-5,POINT);
	Tuple direction(0,0,0, VECTOR);
	Ray ray(origin, direction);
	Sphere sp;
	Matrix newM(sp.getMatrix());
	(void) newM;
	newM.display();
	newM.scale(1, 1, 1);
	newM.display();
	newM.invertedMatrix();
	sp.setIMatrix(newM);
	// float canvas_pixels = 400;
	float wall_z = 10;
	float wall_size = 7;
	float pixel_size = wall_size / 400;
	float half = wall_size /2;
	for (int y = 0; y < 399; y++)
	{
		float world_y = half - (pixel_size * y);
		for (int x = 0; x < 399; x++)
		{
			float world_x = -half + (pixel_size * x);
			Tuple position(world_x, world_y, wall_z, POINT);
			position = position - ray.getOrigin();
			position.normalize();
			ray.setDirection(position);
			if (sp.intersect(&ray))
			{
				img.pixel_put(x,y,red);
			}
		}
	}
	xcb.loop();

	/*======== MATH TESTS=========*/
	t_f4 a = {8, 7, -6, -3};
	t_f4 b = {-5, 5, 0, 0};
	t_f4 c = {9, 6, 9, -9};
	t_f4 d = {2, 1, 6, -4};
	Matrix m1(a,b,c,d);
	m1.display();
	Matrix m2 = m1.invertedMatrix();
	m2.display();
	// Tuple  p(2, 3, 4, 1);
	// Tuple v(1, 0, 0, 0);
	// Ray r(p,v );
	// // Vector pp(1, -2, 3);
	// // pp = pp * 3.5;
	// // Tuple pp = r.position(2.5);
	// // pp.display();

	// std::cout << "NEW TEST FOR DOT PRODUCT" << std::endl;

	// Tuple a(1,2,3,0);
	// Tuple b(2,3,4,0);

	// std::cout << "product " << a.dot_product(b) << std::endl;

	// std::cout << "NEW TEST FOR SPHERE" << std::endl;

	// Tuple pp(0, 1, -5, POINT);
	// Tuple vv(0, 0, 1, VECTOR);
	// Ray newR(pp, vv);
	// Sphere sp;
	// sp.intersect(&newR);

	// std::cout << "NEW TEST FOR NORMALIZATION" << std::endl;
	// Tuple ve(1,2,3, VECTOR);
	// ve.normalize();
	// ve.display();

}