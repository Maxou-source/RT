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
#include "Light.hpp"
#include "World.hpp"
#include "Intersection.hpp"
#include "Computations.hpp"


/*
-Optimise Matrix operations (notably tuple * matrix)
(almost done check later if row accessor is smart)
-correctly implement normal at (much better)
-store intersections duuuuuh
-make a better normalize method (shit doesnt make sense)	
*/
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

Tuple color_at(World *w, Ray *r)
{
	std::set<Intersection>	interSet;
	Light l;
	t_material m;

	interSet = w->intersectWorld(r);
	if (interSet.empty())
		return Tuple(0,0,0,POINT);

	// Tuple cl = l.lighting(m, p, Tuple::negating(ray.getDirection()), sp.normal(p));

	Computations comps(*interSet.begin(), r);
	comps.setPoint(comps.getPoint() + (comps.getNormalv() * EPSILON * 100.0));

	m.color = Tuple(0,0,1, POINT);
	return l.lighting(m, comps.getPoint(), comps.getEyev(), comps.getNormalv());
}

int main()
{
	/*====== GRAPHICAL TESTS======*/
	// setting up graphic stuff
	// World w;
	// w.add_sphere(1, 0,0,0);
	// // w.add_sphere(0.5, 0,0,0);
	// Ray r(Tuple(0,0,0, POINT), Tuple(0,0,1, VECTOR));
	// std::set<Intersection> myset = w.intersectWorld(&r);
	// // myset.printIntersection();
	// std::cout << myset << std::endl;
	// Computations comps(*myset.begin(), &r);
	// std::cout << comps << std::endl;
	// XCB xcb;
	// xcb.setupConnection();
	// xcb.setupScreenAndFormat();
	// xcb.createWindowAndGC();
	// Image img(xcb.getFormatPtr(), 400, 400);
	// xcb.setImage(img);


	// Tuple origin(0,0,-5,POINT);
	// Ray ray(Tuple(0,0,-5, POINT), Tuple(0,0,0,VECTOR));

	// Sphere sp;
	// World w;
	// w.add_sphere(0.5, 0,0,1);
	// // sp.translate(0, 0, 1);
	// // sp.scale(2 , 2, 2);
	// // sp.applyTransformations();
	// // Light l;
	// // t_material m;

	// float wall_z = 10;
	// float wall_size = 7;
	// float pixel_size = wall_size / 400;
	// float half = wall_size /2;

	// for (int y = 0; y < 399; y++)
	// {
	// 	float world_y = half - (pixel_size * y);
	// 	for (int x = 0; x < 399; x++)
	// 	{
	// 		float world_x = -half + (pixel_size * x);
	// 		// part of ray_for_pixel later
	// 		Tuple position(world_x, world_y, wall_z, POINT);
	// 		ray.setDirection(Tuple::normalize(position - ray.getOrigin()));

	// 		img.pixel_put(x,y,float_to_rgba(color_at(&w, &ray).getValue()));
	// 		// here is where i need the intersections
	// 		// std::set<Intersection> myset = sp.intersect(&ray);

	// 		// if (!myset.empty())
	// 		// {
	// 		// 	Tuple p = ray.position((*myset.begin()).getT());
	// 		// 	Light l;
	// 		// 	t_material m;
	// 		// 	m.color = Tuple(0,0,1, POINT);
	// 		// 	Tuple cl = l.lighting(m, p, Tuple::negating(ray.getDirection()), sp.normal(p));
	// 		// 	img.pixel_put(x,y,float_to_rgba(cl.getValue()));
	// 		// }
	// 	}
	// }
	// xcb.loop();

	/*======== MATH TESTS=========*/
	// t_f4 a = {8, 7, -6, -3};
	// t_f4 b = {-5, 5, 0, 0};
	// t_f4 c = {9, 6, 9, -9};
	// t_f4 d = {2, 1, 6, -4};
	// Matrix m1(a,b,c,d);
	// m1.display();
	// Matrix m2 = m1.invertedMatrix();
	// m2.display();


	// World w;
	// w.add_sphere(1, 0,0,0);
	// Tuple nor = sp.normal(Tuple(0.5,0.5,0.5, POINT));
	// nor.display();
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