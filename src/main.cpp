#include <string>
#include <iostream>
#include <stdlib.h>
#include "Sphere.hpp"
#include "parse.hpp"
#include "Matrix.hpp"
#include "rt.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "Light.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "OpenGLObject.hpp"
#include <string>

#include <iostream>
#include <filesystem>
#include <vector>

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
		}
	}
	catch (...) {return 1;}
}

