#include "OpenGLObject.hpp"
#include "Pattern.hpp"
#include "ShaderLoader.hpp"
#include "Texture.hpp"
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glu.h>
constexpr float OpenGLObject::vertices[12];
constexpr unsigned int OpenGLObject::indices[6];

OpenGLObject::OpenGLObject()
{
	glfwSetErrorCallback([](int code, const char* desc) {
	std::cerr << "GLFW error " << code << ": " << desc << std::endl;
	});

	if (!glfwInit())
	{
		std::cerr << "Failed to init GLFW" << std::endl;
		return ;
	}
	// setting up graphic stuff

	// Request an OpenGL 4.3+ core context (compute shaders need 4.3 minimum)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "raytracer", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		return ;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to init GLAD" << std::endl;
		return ;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); 

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	
	float vertices[] = {
		1.0f,  1.0f, 0.0f,  // top right
		1.0f, -1.0f, 0.0f,  // bottom right
	   -1.0f, -1.0f, 0.0f,  // bottom left
	   -1.0f,  1.0f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
	   0, 1, 3,   // first triangle
	   1, 2, 3    // second triangle
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "constructor" << std::endl;
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while actructive	 uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		exit(1);
	}
}

bool OpenGLObject::SetVertexShader()
{
	const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "devut" << std::endl;
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while actructive	 uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		exit(1);
	}
	return true;
}

bool OpenGLObject::SetFragmentShader(const std::string& rootFile)
{
	ShaderLoader loader;
	std::string  source;

	if (!loader.load(rootFile, {
			{"MAX_OBJECTS",  std::to_string(MAX_OBJECTS)},
			{"MAX_LIGHTS",   std::to_string(MAX_LIGHTS)},
			{"EPSILON",   std::to_string(EPSILON)},
			{"OBJ_SPHERE",   std::to_string((int)ObjectId::SPHERE)},
			{"OBJ_CYLINDER", std::to_string((int)ObjectId::CYLINDER)},
			{"OBJ_CUBE", std::to_string((int)ObjectId::CUBE)},
			{"OBJ_PLANE", std::to_string((int)ObjectId::PLANE)},
			{"OBJ_CONE", std::to_string((int)ObjectId::CONE)},
			{"NO_PATTERN",   std::to_string((int)PatternType::NO_PATTERN)},
			{"STRIPE",   std::to_string((int)PatternType::STRIPE)},
			{"CHECKER",   std::to_string((int)PatternType::CHECKER)},
			{"GRADIENT",   std::to_string((int)PatternType::GRADIENT)},
			{"PERLIN",   std::to_string((int)PatternType::PERLIN)},
		}, source))
		return false;

	std::vector<std::string> shaderFiles = loader.files();

	const char* src = source.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &src, NULL);
	glCompileShader(fragmentShader);

	GLint success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[2048];                                // 512 is easily too small for GLSL errors
		glGetShaderInfoLog(fragmentShader, sizeof(log), NULL, log);
		std::cerr << "KO:: fragment shader compilation failed\n" << log << std::endl;
		for (size_t i = 0; i < shaderFiles.size(); i++)
			std::cerr << "  file " << i << " = " << shaderFiles[i] << std::endl;
		glDeleteShader(fragmentShader);
		return false;
	}
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "devut" << std::endl;
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while actructive	 uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		exit(1);
	}
	return true;
}
bool OpenGLObject::Whatever()
{
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	glGenVertexArrays(1, &VAO);  


	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 


	const int width = WIN_WIDTH, height = WIN_HEIGHT;
	const int numPixels = width * height;

	// directive to gpu use the layout binding at index 0
	glGenBuffers(1, &debugSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);
	const int slotsPerPixel = 12;
	// "allocate" the memory at this address
	glBufferData(GL_SHADER_STORAGE_BUFFER,
				(size_t)numPixels * slotsPerPixel * sizeof(float) * 4,
				nullptr, GL_DYNAMIC_READ);
	
	// bind it with the allocated size I guess
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, debugSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "devut" << std::endl;
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while actructive	 uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		exit(1);
	}
	return true;
}

bool OpenGLObject::UploadTextures(const std::vector<Image>& imgs)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "devut" << std::endl;
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while actructive	 uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		return false;
	}
	if (imgs.empty())
		return true;                           // no textures in the scene: nothing to do

	// GLint maxLayers = 0;
	// glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);
	// 	if (err != GL_NO_ERROR)
	// {
	// 	std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
	// 	          << " while actiddddve	 uploading textures" << std::endl;
	// 	const GLubyte* errorString = gluErrorString(err);
    // 	printf("OpenGL Error: %s\n", errorString);
	// 	return false;
	// }
	// if ((GLint)imgs.size() > maxLayers)
	// {
	// 	std::cerr << "KO:: too many textures (" << imgs.size()
	// 	          << ", max " << maxLayers << ")" << std::endl;
	// 	return false;
	// }

	if (texArray)
		glDeleteTextures(1, &texArray);        // re-upload on reload

	glGenTextures(1, &texArray);
	if (err != GL_NO_ERROR)
	{
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while actructive	 uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		return false;
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, texArray);

	// int texsize = img.;
	int levels = 1 + (int)std::floor(std::log2(TEX_SIZE));
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 4, GL_SRGB8_ALPHA8,
	               TEX_SIZE, TEX_SIZE, (GLsizei)imgs.size());

	for (size_t i = 0; i < imgs.size(); i++)
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, (GLint)i,
		                TEX_SIZE, TEX_SIZE, 1,
		                GL_RGBA, GL_UNSIGNED_BYTE, imgs[i].px.data());

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	std::cout << "shaderProga" << shaderProgram << std::endl;
	glUseProgram(shaderProgram);
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (err != GL_NO_ERROR)
	{
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " whiledddd active	 uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		return false;
	}
	if (!success) {
		// 2. Allocate space for the error message
		GLint logLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		
		if (logLength > 0) {
			std::vector<GLchar> infoLog(logLength);
			glGetProgramInfoLog(shaderProgram, logLength, nullptr, infoLog.data());
			
			// 3. Print the explicit compiler/linker error
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" 
					<< infoLog.data() << "\n" << std::endl;
		}
	} else {
		std::cout << "Shader program linked successfully!" << std::endl;
	}
	glActiveTexture(GL_TEXTURE0);
	if (err != GL_NO_ERROR)
	{
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while active	 uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		return false;
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, texArray);
	if (err != GL_NO_ERROR)
	{
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while binding uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		return false;
	}
	glUniform1i(glGetUniformLocation(shaderProgram, "textures"), 0);

	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		return false;
	}
	return true;
}

void OpenGLObject::UploadScene(const GpuScene& scene)
{
	GLenum err = glGetError();
	if (!ubo)
	{
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(GpuScene),
		             &scene, GL_DYNAMIC_DRAW);
		GLuint idx = glGetUniformBlockIndex(shaderProgram, "Scene");
		glUniformBlockBinding(shaderProgram, idx, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
	}
	else
	{
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GpuScene), &scene);
	}
	if (err != GL_NO_ERROR)
	{
		std::cout << "devut" << std::endl;
		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
		          << " while actructive	 uploading textures" << std::endl;
		const GLubyte* errorString = gluErrorString(err);
    	printf("OpenGL Error: %s\n", errorString);
		exit(1);
	}
}

void OpenGLObject::UploadCamera(const GpuScene& scene)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, offsetof(GpuScene, cam), sizeof(GpuCamera), &scene.cam);
}




/*============= SETTERS && GETTERS ===========*/


const char* OpenGLObject::getInfoLog() const {
    return infoLog;
}

void OpenGLObject::setInfoLog(const char* log) {
    if (log != nullptr) {
        strncpy(infoLog, log, sizeof(infoLog) - 1);
        infoLog[sizeof(infoLog) - 1] = '\0';
    }
}

// vertexShader getters and setters
unsigned int OpenGLObject::getVertexShader() const {
    return vertexShader;
}

void OpenGLObject::setVertexShader(unsigned int shader) {
    vertexShader = shader;
}

// fragmentShader getters and setters
unsigned int OpenGLObject::getFragmentShader() const {
    return fragmentShader;
}

void OpenGLObject::setFragmentShader(unsigned int shader) {
    fragmentShader = shader;
}

// shaderProgram getters and setters
unsigned int OpenGLObject::getShaderProgram() const {
    return shaderProgram;
}

void OpenGLObject::setShaderProgram(unsigned int program) {
    shaderProgram = program;
}

// VBO getters and setters
unsigned int OpenGLObject::getVBO() const {
    return VBO;
}

void OpenGLObject::setVBO(unsigned int vbo) {
    VBO = vbo;
}

// EBO getters and setters
unsigned int OpenGLObject::getEBO() const {
    return EBO;
}

void OpenGLObject::setEBO(unsigned int ebo) {
    EBO = ebo;
}

// VAO getters and setters
unsigned int OpenGLObject::getVAO() const {
    return VAO;
}

void OpenGLObject::setVAO(unsigned int vao) {
    VAO = vao;
}

// debugSSBO getters and setters
unsigned int OpenGLObject::getDebugSSBO() const {
    return debugSSBO;
}

void OpenGLObject::setDebugSSBO(unsigned int ssbo) {
    debugSSBO = ssbo;
}

// window getters and setters
GLFWwindow* OpenGLObject::getWindow() const {
    return window;
}

void OpenGLObject::setWindow(GLFWwindow* win) {
    window = win;
}

void OpenGLObject::setUbo(GLuint gl) {
	ubo = gl;
}

GLuint OpenGLObject::getUbo(void) const {
	return ubo;
}