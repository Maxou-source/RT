#pragma once 
#include "rt.hpp"
#include <glad/glad.h>   // must be included BEFORE glfw3.h
#include <GLFW/glfw3.h>

class OpenGLObject
{
	public:
		OpenGLObject();
		bool SetVertexShader();
		bool SetFragmentShader(const char *fileName);
		bool Whatever();

		// Setters and getters
		unsigned int getShaderProgram() const;
		void setShaderProgram(unsigned int);

		const char* getInfoLog() const;
		void setInfoLog(const char*);

		unsigned int getVertexShader() const;
		void setVertexShader(unsigned int);

		unsigned int getFragmentShader() const;
		void setFragmentShader(unsigned int);

		unsigned int getVBO() const;
		void setVBO(unsigned int);

		unsigned int getEBO() const;
		void setEBO(unsigned int);

		unsigned int getVAO() const;
		void setVAO(unsigned int);

		unsigned int getDebugSSBO() const;
		void setDebugSSBO(unsigned int);

GLFWwindow* getWindow() const;
void setWindow(GLFWwindow*);
	private:
		char infoLog[512];
		unsigned int vertexShader;
		unsigned int fragmentShader;

		unsigned int shaderProgram;

		unsigned int VBO;
		unsigned int EBO;

		unsigned int VAO;
		unsigned int debugSSBO;

		GLFWwindow* window;

		static constexpr float vertices[12] = {
			1.0f,  1.0f, 0.0f,  // top right
			1.0f, -1.0f, 0.0f,  // bottom right
			-1.0f, -1.0f, 0.0f,  // bottom left
			-1.0f,  1.0f, 0.0f   // top left 
		};
		static constexpr unsigned int indices[6] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
};