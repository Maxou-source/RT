#pragma once 
#include "rt.hpp"
#include <glad/glad.h>   // must be included BEFORE glfw3.h
#include <GLFW/glfw3.h>
#include <chrono>
#include "GpuObjects.hpp"
#include "Texture.hpp"
#include "World.hpp"

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

struct Movements {
	float fwd = 0;
	float rgt = 0;
	float upw = 0;

	float dYaw = 0;
	float dPitch = 0;
};

struct Time {
	double	last;
	float	now;
};

class OpenGLObject
{
	public:
		OpenGLObject();
		bool Whatever();
		
		bool SetComputeShader(const std::string& rootFile);
		void CreateOutputTexture(int w, int h);

		void handleResize();

		// upload data
		bool UploadScene(const GpuScene& scene);
		void UploadCamera(const GpuScene& scene);
		bool UploadWorld(World *w);
		bool UploadTextures(const std::vector<Image>& imgs);
		
		// update camera
		void UpdateCamera();

		// Setters and getters
		unsigned int getComputeProgram() const;
		void setComputeProgram(unsigned int);

		const char* getInfoLog() const;
		void setInfoLog(const char*);

		unsigned int getComputeShader() const;
		void setComputeShader(unsigned int);

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

		void setUbo(GLuint gl);

		GLuint getUbo(void) const;
		GLuint getReadFBO() const {return readFBO;}

		void handleMovements();
		void handleChange();
		void Render();
	
		private:
			// OPENGL stuff
			GLuint outTex  = 0;
			GLuint readFBO = 0;
			GLuint texArray = 0;
			char infoLog[512];
			GLuint computeShader;
			GLuint computeProgram;
			GLuint debugSSBO;
			GLuint ubo = 0;

			GLFWwindow*	window;

			// time
			Time		time;

			// movements handle
			Movements	mov;
			
			// WORLD
			// movement factors
			GpuScene	gpuS;
			World		*world;
			Camera		cam;
			float		baseSpeed;
			float		speedMul;

			// actual size of the window
			int			actualHeight;
			int			actualWidth;

			bool		needsRender;
			bool		changed = false;

};