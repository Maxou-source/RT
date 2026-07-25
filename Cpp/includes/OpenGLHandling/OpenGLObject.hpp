#pragma once 
#include <glad/glad.h>   // must be included BEFORE glfw3.h
#include <GLFW/glfw3.h>

class OpenGLObject
{
	public:
		OpenGLObject();
		bool SetVertexShader();
		bool SetFragmentShader(const char *fileName);
		bool Whatever();

	private:
		char infoLog[512];
		unsigned int vertexShader;
		unsigned int fragmentShader;

		GLFWwindow* window;
};