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

void debugMessages(OpenGLObject OpenGLObj)
{
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
	std::cout << "transform matrix: " << std::endl;
	for (int i = 0; i < 16; i++)
	{
		std::cout << "" << matrix[i] << ", ";
		if (i % 4 == 3)
			std::cout << std::endl;
	}
	std::cout << std::endl;
	location = glGetUniformLocation(OpenGLObj.getShaderProgram(), "cam.inv_transform");
	glGetUniformfv(OpenGLObj.getShaderProgram(), location, matrix); 
	std::cout << "inv transform matrix: " << std::endl;
	for (int i = 0; i < 16; i++)
	{
		std::cout << "" << matrix[i] << ", ";
		if (i % 4 == 3)
			std::cout << std::endl;
	}
	std::cout << std::endl;
}

void printMat4(const float* m, const char* name)
{
    std::cout << colorprint::green << name << colorprint::reset << std::endl;
    for (int row = 0; row < 4; ++row)
    {
        std::cout << "  | ";
        for (int col = 0; col < 4; ++col)
            std::cout << std::setw(10) << std::fixed << std::setprecision(4)
                      << m[col * 4 + row] << " ";
        std::cout << "|" << std::endl;
    }
}

int main()
{
	/*====== GRAPHICAL TESTS======*/

	OpenGLObject OpenGLObj;

	OpenGLObj.SetVertexShader();
	OpenGLObj.SetFragmentShader("shader/raytracer.glsl");
	OpenGLObj.Whatever();
	
	Camera cam(OpenGLObj.getShaderProgram(), Tuple(0,0,-2, POINT), Tuple(0,0,1,VECTOR), 90.0);

	debugMessages(OpenGLObj);

	// World w = buildScene1();

	
	FPSCounter fpsCounter;
	bool one = true;
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
		std::vector<float> debugOut(WIN_HEIGHT * WIN_WIDTH * 4 * 5);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, debugOut.size() * sizeof(float), debugOut.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);	
		
		if (one == true)
		{
			int px = 0, py = 0;
			while (py < WIN_HEIGHT)
			{
				while (px < WIN_WIDTH)
				{
					int idx = (py * WIN_WIDTH + px) * 4;
					if (px != 200 || py != 200)
					{
						px++;
						continue;
					}
					std::cout << colorprint::green <<"CAMERA " << colorprint::reset << "half_width: " << debugOut[idx] << " half_height: " << debugOut[idx+1] << " half_view: " << debugOut[idx+2] << " pixel_size: " << debugOut[idx+3] << std::endl;
					printMat4(&debugOut[idx+1], "CAMERA TRANSFORM");
					px++;
					// std::cout << "normalv: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
					// idx = idx + 4;
					// std::cout << "eyev: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
					// idx+=4;
					// std::cout << "point: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
					// idx+=4;
					// std::cout << "t: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
					// idx+=4;
					// std::cout << "direction: " << debugOut[idx] << ", " << debugOut[idx+1] << ", " << debugOut[idx+2] << ", " << debugOut[idx+3] << std::endl;
					// idx+=4;
				}
				px = 0;
				py++;
			}

		}
		one = false;
	}
	glfwTerminate();
}
