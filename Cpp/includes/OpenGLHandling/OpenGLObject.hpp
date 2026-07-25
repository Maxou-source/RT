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

		// Setters and getters
		unsigned int getShaderProgram() const;
		void setShaderProgram(int);

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
};