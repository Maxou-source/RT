#include <string>
#include <iostream>
#include <stdlib.h>
#include "Sphere.hpp"
#include "Ray.hpp"
#include "parse.hpp"
#include "rt.hpp"
// #include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
// #include <SDL2/SDL.h>
#include <xcb/xcb.h>
#include "XCB.hpp"
// #include <GL/glew.h>
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>


int main()
{

	XCB xcb;
	xcb.setupConnection();
	xcb.setupScreenAndFormat();
	xcb.createWindow();
	xcb.loop();
	// std::cout << "everuthing compiling" << std::endl;
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