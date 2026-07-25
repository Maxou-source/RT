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
#include "OpenGLObject.hpp"
#include <string>

#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>
/*
-Optimise Matrix operations (notably tuple * matrix)
(almost done check later if row accessor is smart)
-correctly implement normal at (much better)
-store intersections duuuuuh
-make a better normalize method (shit doesnt make sense)	
*/

#include <chrono>

class FPSCounter {
public:
    void update() {
        frames++;
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(now - lastTime).count();
        if (elapsed >= 1.0) {
            fps = frames / elapsed;
            frames = 0;
            lastTime = now;
			std::cout << "FPS: " << fps << std::endl;
        }
    }

    double getFPS() const { return fps; }

private:
    int frames = 0;
    double fps = 0.0;
    std::chrono::high_resolution_clock::time_point lastTime =
        std::chrono::high_resolution_clock::now();
};

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
	/*====== GRAPHICAL TESTS======*/

	OpenGLObject OpenGLObj;

	OpenGLObj.SetVertexShader();
	OpenGLObj.SetFragmentShader("shader/raytracer.glsl");
	OpenGLObj.Whatever();
	
	Camera cam(OpenGLObj.getShaderProgram(), Tuple(0,0,-2, POINT), Tuple(0,0,-1,VECTOR), 90.0);
	
	GLfloat value;
	std::cout << std::endl << "shaderProgram: " << OpenGLObj.getShaderProgram() << std::endl;
	
	auto location = glGetUniformLocation(OpenGLObj.getShaderProgram(), "cam.half_view");
	glGetUniformfv(OpenGLObj.getShaderProgram(), location, &value);
	std::cout << "half_view value: " << value << std::endl;

	location = glGetUniformLocation(OpenGLObj.getShaderProgram(), "cam.half_height");
	glGetUniformfv(OpenGLObj.getShaderProgram(), location, &value);
	std::cout << "half_view half_height: " << value << std::endl;

	 location = glGetUniformLocation(OpenGLObj.getShaderProgram(), "cam.half_width");
	glGetUniformfv(OpenGLObj.getShaderProgram(), location, &value);
	std::cout << "half_width value: " << value << std::endl;


	GLfloat matrix[16];
	GLfloat ok;

	location = glGetUniformLocation(OpenGLObj.getShaderProgram(), "cam.transform");
	glGetUniformfv(OpenGLObj.getShaderProgram(), location, matrix);
	for (int i = 0; i < 16; i++)
	{
		std::cout << "" << matrix[i] << ", ";
		if (i % 4 == 3)
			std::cout << std::endl;
	}
	std::cout << std::endl;
	location = glGetUniformLocation(OpenGLObj.getShaderProgram(), "cam.inv_transform");
	glGetUniformfv(OpenGLObj.getShaderProgram(), location, matrix); 
	std::cout << "transform matrix: " << matrix[0] << ", " << matrix[1] << ", " << matrix[2] << ", " << matrix[3] << std::endl;
	for (int i = 0; i < 16; i++)
	{
		std::cout << "" << matrix[i] << ", ";
		if (i % 4 == 3)
			std::cout << std::endl;
	}
	std::cout << std::endl;
	World w = buildScene1();

	bool one = true;

	FPSCounter fpsCounter;
	while (!glfwWindowShouldClose(OpenGLObj.getWindow()))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(OpenGLObj.getShaderProgram());
		glBindVertexArray(OpenGLObj.getVAO());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(OpenGLObj.getWindow());
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, OpenGLObj.getDebugSSBO());
	std::vector<float> debugOut(WIN_HEIGHT * WIN_WIDTH * 4);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, debugOut.size() * sizeof(float), debugOut.data());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);	

	// inspect one pixel, e.g. (200,200)

	fpsCounter.update();
	
	if (one == true)
	{
		int px = 0, py = 0;
		while (py < WIN_HEIGHT)
		{
			while (px < WIN_WIDTH)
			{
				int idx = (py * WIN_WIDTH + px) * 4;
				// if (debugOut[idx+3] == 0.f)
				// {
				// 	px++;
				// 	continue;
				// }
				// std::cout << "pixel (" << px << "," << py << ") origin: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2]
				// << std::endl;
				// std::cout << "wheere = " << debugOut[idx+3] << std::endl;
				// px++;
				if (px != 200 || py != 200)
				{
					px++;
					continue;
				}
				std::cout << ") normalv: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
				idx = idx + 4;
				std::cout << ") eyev: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
				idx+=4;
				std::cout << ") point: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
				idx+=4;
				std::cout << ") t: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
				idx+=4;
				std::cout << ") direction: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
				px++;
			}
			px = 0;
			py++;
		}

	}
	one = false;
	}
// exit(0);

	glfwTerminate();

	XCB xcb;
	xcb.setupConnection();
	xcb.setupScreenAndFormat();
	xcb.createWindowAndGC();
	Image img(xcb.getFormatPtr(), 1080, 1920);
	xcb.setImage(img);

	Tuple origin(0,0,-5,POINT);
	Ray ray(Tuple(0,0,0, POINT), Tuple(0,0,0,VECTOR));

	// World w = buildScene1();

	
	
	// setcameraobject for opengl
	// open .glsl
	while (1)
	{
	
		for (int y = 0; y < 1920; y++)
		{
			for (int x = 0; x < 1080; x++)
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

		fpsCounter.update();

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
