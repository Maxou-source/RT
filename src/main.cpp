#include <string>
#include <iostream>
#include <stdlib.h>
#include "Sphere.hpp"
#include "parse.hpp"
#include "Matrix.hpp"
#include "rt.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include "Light.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "OpenGLObject.hpp"
#include <string>

#include <iostream>
#include <filesystem>
#include <vector>

// GpuScenePrinter.hpp / .cpp
#include <iostream>
#include <iomanip>

int main(int ac, char **av)
{
	/*====== PARSING ======*/
	try {

		World	w;
		Parse	par(&w);
	
		if (ac != 2 || par.parseFileAndName(av[1]))
		{
			std::cerr << "KO:: Wrong number of arguments" << std::endl;
			return (1);
		}
	
		if (!par.getBase()) {
			std::cerr << "KO:: Something wrong with your file" << std::endl;
			return (1);
		}
	
		/*====== SETTING UP ======*/
		// setup OpenGLObject
		OpenGLObject OpenGLObj;
		
		if (!OpenGLObj.SetComputeShader("shader/raytracer.glsl"))
			return (-1);
		if (!OpenGLObj.LinkComputeProgram())
			return (-1);
	
	
		// filling data in OpenGLObject
		if (!OpenGLObj.UploadWorld(&w))
			return 1;
	
		bool one = true;
	
		while (!glfwWindowShouldClose(OpenGLObj.getWindow()))
		{
			OpenGLObj.handleResize();
			glfwPollEvents();
	
			// fill in Movements struct if a movement occured
			OpenGLObj.handleMovements();
	
			// change 
			OpenGLObj.handleChange();
	
			// render
			OpenGLObj.Render();
	
			if (one == true)
			{
				std::vector<float> debugOut(WIN_HEIGHT * WIN_WIDTH * 4 * 12);
				glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, debugOut.size() * sizeof(float), debugOut.data());
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
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
						std::cout << colorprint::green <<"CAMERA :: " << colorprint::reset << "half_width: " << debugOut[idx] << " half_height: " << debugOut[idx+1] << " half_view: " << debugOut[idx+2] << " pixel_size: " << debugOut[idx+3] << std::endl;
						idx = idx + 4;
						// printMat4(&debugOut[idx], "CAMERA TRANSFORM :: ");
						idx = idx + 16;
						// printMat4(&debugOut[idx], "CAMERA TRANSFORM :: ");
						// idx = idx + 16;
						std::cout << colorprint::red << "RAY_FOR_PIXEL :: " << colorprint::reset << std::endl;
						std::cout << " world_point x " << debugOut[idx+1] << ", y" << debugOut[idx+2] << ", z" << debugOut[idx+3] << "w " << debugOut[idx+4] << std::endl;
						idx = idx + 4;
						std::cout << " r.origin x " << debugOut[idx+1] << ", y" << debugOut[idx+2] << ", z" << debugOut[idx+3] << "w " << debugOut[idx+4] << std::endl;
						idx = idx + 4;
						std::cout << " r.direction x " << debugOut[idx+1] << ", y" << debugOut[idx+2] << ", z" << debugOut[idx+3] << "w " << debugOut[idx+4] << std::endl;
						px++;
					}
					px = 0;
					py++;
				}
	
			}
			one = false;
		}
	}
	catch (...) {return 1;}
}

