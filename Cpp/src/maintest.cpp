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
#include <glad/glad.h>   // must be included BEFORE glfw3.h
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <vector>
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

float vertices[] = {
	0.5f,  0.5f, 0.0f,  // top right
	0.5f, -0.5f, 0.0f,  // bottom right
   -0.5f, -0.5f, 0.0f,  // bottom left
   -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
   0, 1, 3,   // first triangle
   1, 2, 3    // second triangle
};

int main()
{
	/*====== GRAPHICAL TESTS======*/
	if (!glfwInit())
	{
		std::cerr << "Failed to init GLFW" << std::endl;
		return 1;
	}
	// setting up graphic stuff

	// Request an OpenGL 4.3+ core context (compute shaders need 4.3 minimum)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(400, 400, "raytracer", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create window caca" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to init GLAD" << std::endl;
		return 1;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	unsigned int VBO;
	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
		std::cout << "Success" << std::endl;


	std::ifstream shaderFile;

	
	shaderFile.open("shader/raytracer.glsl");
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	std::string fragment = shaderStream.str();
	const char *fragmentShaderSource = fragment.c_str();
	std::cout << "lalalal" << fragmentShaderSource << std::endl;

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::frag::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
		std::cout << "Success" << std::endl;

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);  


	glBindVertexArray(VAO);
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 

	glUniform3f(glGetUniformLocation(shaderProgram, "cam.position"), 0.0, 1.0, 0.0);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	exit(0);

	XCB xcb;
	xcb.setupConnection();
	xcb.setupScreenAndFormat();
	xcb.createWindowAndGC();
	Image img(xcb.getFormatPtr(), 400, 400);
	xcb.setImage(img);

	Tuple origin(0,0,-5,POINT);
	Ray ray(Tuple(0,0,0, POINT), Tuple(0,0,0,VECTOR));

	World w = buildScene1();

	Camera cam(Tuple(0,0,-1.5, POINT), Tuple(0,0,-1,VECTOR), 90.0);
	
	
	// setcameraobject for opengl
	// open .glsl

	for (int y = 0; y < 399; y++)
	{
		for (int x = 0; x < 399; x++)
		{
			// CAMERA FIXED VALUE half_width half_height ...
			// rayforpixel(cam, x, y, &ray);
			
			// WORLD FIXED VALUE scene basically
			// color at .... 
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