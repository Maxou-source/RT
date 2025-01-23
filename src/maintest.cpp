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
#include "Camera.hpp"


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
	t_material mat;

	interSet = w->intersectWorld(r);
	if (interSet.empty())
		return Tuple(0,0,0,POINT);

	// Tuple cl = l.lighting(m, p, Tuple::negating(ray.getDirection()), sp.normal(p));

	Computations comps(*interSet.begin(), r);
	comps.setPoint(comps.getPoint() + (comps.getNormalv() * EPSILON * 100.0));

	mat.color = comps.getObjPtr()->getColor();
	return l.lighting(mat, comps.getPoint(), comps.getEyev(), comps.getNormalv());
}

World buildScene1()
{
	World *w = new World();

	// AObject *sp = new Sphere();
	// // sp->scale(10 , 0.01, 10);
	// // sp->translate(0 , 0, 0);
	// // sp->applyTransformations();
	// sp->setColor(Tuple(1,0,0,0));
	// Matrix m0, m00;
	// m0.scale(10, 0.01, 10);
	// m00.translate(0,0,0);
	// sp->setMatrix(m00 * m0);
	// sp->applyTransformations();
	// w->add_object(sp);

	AObject *sp1 = new Sphere();
	Matrix m, m2, m3, m4;
	m.translate(0,0,5);
	// m2.rotation_matrix_y((-PI)/4);
	// m3.rotation_matrix_x(PI/2);
	m4.scale(10, 0.01, 10);
	sp1->setColor(Tuple(0,1,0, 0));
	// sp1->setMatrix(((m2 * m3) * m) * m4);
	sp1->setMatrix((m * m4) * (m2 * m3));
	// sp1->setMatrix(m4 * m3 * m2 * m);
	sp1->applyTransformations();
	w->add_object(sp1);

	// AObject *sp2 = new Sphere();
	// Matrix m5, m6, m7, m8;
	// m5.translate(0,0,5);
	// // m6.rotation_matrix_y(PI/4);
	// // m7.rotation_matrix_x(PI/2);
	// m8.scale(10 , 0.01, 10);
	// sp2->setColor(Tuple(0,0,1, 0));
	// sp2->setMatrix(m5 * m6 * m7 * m8);
	// sp2->applyTransformations();
	// w->add_object(sp2);
	return *w;
}

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


	Tuple origin(0,0,-5,POINT);
	Ray ray(Tuple(0,0,0, POINT), Tuple(0,0,0,VECTOR));

	// Sphere sp;
	World w = buildScene1();
	// w.add_sphere();

	Camera cam(Tuple(0,1.5,-5, POINT), Tuple(0,1,0,VECTOR), 90.0);

	// float wall_z = 10;
	// float wall_size = 7;
	// float pixel_size = wall_size / 400;
	// float half = wall_size /2;

	for (int y = 0; y < 399; y++)
	{
		// float world_y = half - (pixel_size * y);
		for (int x = 0; x < 399; x++)
		{
			cam.rayForPixel(x, y, &ray);
			img.pixel_put(x,y,float_to_rgba(color_at(&w, &ray).getValue()));
		}
	}
	xcb.loop();

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