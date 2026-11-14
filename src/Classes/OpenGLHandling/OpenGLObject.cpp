#include "OpenGLObject.hpp"
#include "Pattern.hpp"
#include "ShaderLoader.hpp"
#include "Texture.hpp"
#include "Light.hpp"
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
	// glGenVertexArrays(1, &VAO);
	// glBindVertexArray(VAO);

	// glGenBuffers(1, &VBO);  
	// glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0); 

	// glGenBuffers(1, &EBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

// bool OpenGLObject::SetVertexShader()
// {
// 	const char *vertexShaderSource = "#version 330 core\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//     "}\0";
	
// 	vertexShader = glCreateShader(GL_VERTEX_SHADER);

// 	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
// 	glCompileShader(vertexShader);

// 	int  success;
// 	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

// 	if(!success)
// 	{
// 		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
// 		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
// 		return false;
// 	}
// 	GLenum err = glGetError();
// 	if (err != GL_NO_ERROR)
// 	{
// 		std::cout << "devut" << std::endl;
// 		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
// 		          << " while actructive	 uploading textures" << std::endl;
// 		const GLubyte* errorString = gluErrorString(err);
//     	printf("OpenGL Error: %s\n", errorString);
// 		exit(1);
// 	}
// 	return true;
// }


bool OpenGLObject::SetComputeShader(const std::string& rootfile)
{
    ShaderLoader loader;
    std::string source;

    if (!loader.load(rootfile, {
        {"MAX_OBJECTS", std::to_string(MAX_OBJECTS)},
        {"MAX_LIGHTS", std::to_string(MAX_LIGHTS)},
        {"EPSILON", std::to_string(EPSILON)},
        {"OBJ_SPHERE", std::to_string((int)ObjectId::SPHERE)},
        {"OBJ_CYLINDER", std::to_string((int)ObjectId::CYLINDER)},
        {"OBJ_CUBE", std::to_string((int)ObjectId::CUBE)},
        {"OBJ_TORUS", std::to_string((int)ObjectId::TORUS)},
        {"OBJ_PLANE", std::to_string((int)ObjectId::PLANE)},
        {"OBJ_CONE", std::to_string((int)ObjectId::CONE)},
        {"OBJ_PARABOLOID", std::to_string((int)ObjectId::PARABOLOID)},
        {"NO_PATTERN", std::to_string((int)PatternType::NO_PATTERN)},
        {"STRIPE", std::to_string((int)PatternType::STRIPE)},
        {"CHECKER", std::to_string((int)PatternType::CHECKER)},
        {"GRADIENT", std::to_string((int)PatternType::GRADIENT)},
        {"RING", std::to_string((int)PatternType::RING)},
        {"PERLIN", std::to_string((int)PatternType::PERLIN)},
        {"DIRECTIONAL", std::to_string((int)LightType::DIRECTIONAL)},
        {"POSITIONAL", std::to_string((int)LightType::POSITIONAL)},
        {"CSG_UNION", std::to_string((int)ObjectId::CSG_UNION)},
        {"CSG_DIFFERENCE", std::to_string((int)ObjectId::CSG_DIFFERENCE)},
        {"CSG_INTERSECT", std::to_string((int)ObjectId::CSG_INTERSECT)},
    }, source))
        return false;

    computeShader = glCreateShader(GL_COMPUTE_SHADER);

    const char* src = source.c_str();
    glShaderSource(computeShader, 1, &src, nullptr);
    glCompileShader(computeShader);

    GLint success = GL_FALSE;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint length = 0;
        glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &length);

        std::vector<char> log(length);
        glGetShaderInfoLog(computeShader, length, nullptr, log.data());

        std::cerr << "COMPUTE SHADER COMPILATION FAILED:\n"
                  << log.data() << std::endl;

        glDeleteShader(computeShader);
        computeShader = 0;
        return false;
    }

    std::cout << "Compute shader compiled successfully!" << std::endl;
    return true;
}

void OpenGLObject::CreateOutputTexture(int w, int h)
{
	// 1. free the old texture (resize case)
	if (outTex)
		glDeleteTextures(1, &outTex);

	// 2. immutable storage, filled by the compute shader
	glGenTextures(1, &outTex);
	glBindTexture(GL_TEXTURE_2D, outTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);

	// 3. filtering (only matters if the blit scales)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 4. image unit 0, must match layout(rgba8, binding = 0) in the shader
	glBindImageTexture(0, outTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	// 5. FBO used to read the texture in glBlitFramebuffer
	if (!readFBO)
		glGenFramebuffers(1, &readFBO);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, readFBO);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	                       GL_TEXTURE_2D, outTex, 0);
glReadBuffer(GL_COLOR_ATTACHMENT0);
	// 6. completeness check
	if (glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "KO:: read framebuffer is not complete" << std::endl;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);   // leave the state clean
	glBindTexture(GL_TEXTURE_2D, 0);
}

// bool OpenGLObject::SetFragmentShader(const std::string& rootFile)
// {
// 	ShaderLoader loader;
// 	std::string  source;

// 	if (!loader.load(rootFile, {
// 			{"MAX_OBJECTS",  std::to_string(MAX_OBJECTS)},
// 			{"MAX_LIGHTS",   std::to_string(MAX_LIGHTS)},
// 			{"EPSILON",   std::to_string(EPSILON)},
// 			{"OBJ_SPHERE",   std::to_string((int)ObjectId::SPHERE)},
// 			{"OBJ_CYLINDER", std::to_string((int)ObjectId::CYLINDER)},
// 			{"OBJ_CUBE", std::to_string((int)ObjectId::CUBE)},
// 			{"OBJ_TORUS", std::to_string((int)ObjectId::TORUS)},
// 			{"OBJ_PLANE", std::to_string((int)ObjectId::PLANE)},
// 			{"OBJ_CONE", std::to_string((int)ObjectId::CONE)},
// 			{"OBJ_PARABOLOID", std::to_string((int)ObjectId::PARABOLOID)},
// 			{"NO_PATTERN",   std::to_string((int)PatternType::NO_PATTERN)},
// 			{"STRIPE",   std::to_string((int)PatternType::STRIPE)},
// 			{"CHECKER",   std::to_string((int)PatternType::CHECKER)},
// 			{"GRADIENT",   std::to_string((int)PatternType::GRADIENT)},
// 			{"RING",   std::to_string((int)PatternType::RING)},
// 			{"PERLIN",   std::to_string((int)PatternType::PERLIN)},
// 			{"DIRECTIONAL",   std::to_string((int)LightType::DIRECTIONAL)},
// 			{"POSITIONAL", std::to_string((int)LightType::POSITIONAL)},
// 			{"CSG_UNION", std::to_string((int)ObjectId::CSG_UNION)},
// 			{"CSG_DIFFERENCE", std::to_string((int)ObjectId::CSG_DIFFERENCE)},
// 			{"CSG_INTERSECT", std::to_string((int)ObjectId::CSG_INTERSECT)},
// 		}, source))
// 		return false;

// 	std::vector<std::string> shaderFiles = loader.files();

// 	const char* src = source.c_str();
// 	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
// 	glShaderSource(fragmentShader, 1, &src, NULL);
// 	glCompileShader(fragmentShader);

// 	GLint success;
// 	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
// 	if (!success)
// 	{
// 		char log[2048];                                // 512 is easily too small for GLSL errors
// 		glGetShaderInfoLog(fragmentShader, sizeof(log), NULL, log);
// 		std::cerr << "KO:: fragment shader compilation failed\n" << log << std::endl;
// 		for (size_t i = 0; i < shaderFiles.size(); i++)
// 			std::cerr << "  file " << i << " = " << shaderFiles[i] << std::endl;
// 		glDeleteShader(fragmentShader);
// 		return false;
// 	}
// 	GLenum err = glGetError();
// 	if (err != GL_NO_ERROR)
// 	{
// 		std::cout << "devut" << std::endl;
// 		std::cerr << "KO:: GL error 0x" << std::dec << err << std::dec
// 		          << " while actructive	 uploading textures" << std::endl;
// 		const GLubyte* errorString = gluErrorString(err);
//     	printf("OpenGL Error: %s\n", errorString);
// 		exit(1);
// 	}
// 	return true;
// }

bool OpenGLObject::Whatever()
{
    computeProgram = glCreateProgram();

    glAttachShader(computeProgram, computeShader);
    glLinkProgram(computeProgram);

    GLint success = GL_FALSE;
    glGetProgramiv(computeProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLint length = 0;
        glGetProgramiv(computeProgram, GL_INFO_LOG_LENGTH, &length);

        std::vector<char> log(length);
        glGetProgramInfoLog(computeProgram, length, nullptr, log.data());

        std::cerr << "COMPUTE PROGRAM LINK FAILED:\n"
                  << log.data() << std::endl;

        glDeleteProgram(computeProgram);
        computeProgram = 0;
        glDeleteShader(computeShader);
        computeShader = 0;

        return false;
    }

    glDetachShader(computeProgram, computeShader);
    glDeleteShader(computeShader);
    computeShader = 0;

    std::cout << "Compute program linked successfully!" << std::endl;

    const int width = WIN_WIDTH;
    const int height = WIN_HEIGHT;
    const int numPixels = width * height;

    glGenBuffers(1, &debugSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);

    const int slotsPerPixel = 12;

    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        (size_t)numPixels * slotsPerPixel * sizeof(float) * 4,
        nullptr,
        GL_DYNAMIC_READ
    );

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, debugSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    return true;
}

bool CheckGLError(const char* operation)
{
    GLenum err;
    bool success = true;

    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error during "
                  << operation << ": 0x"
                  << std::hex << err << std::dec
                  << " (" << gluErrorString(err) << ")"
                  << std::endl;

        success = false;
    }

    return success;
}

bool OpenGLObject::UploadTextures(const std::vector<Image>& imgs)
{
    if (imgs.empty())
        return true;

    GLint maxLayers = 0;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);

    if (imgs.size() > (size_t)maxLayers)
    {
        std::cerr << "Too many texture layers!" << std::endl;
        return false;
    }

    // All images must have identical dimensions
    int width = imgs[0].w;
    int height = imgs[0].h;

    if (width <= 0 || height <= 0)
        return false;

    for (const auto& img : imgs)
    {
        if (img.w != width || img.h != height)
        {
            std::cerr << "Inconsistent texture dimensions!" << std::endl;
            return false;
        }

        if (img.px.size() != (size_t)width * height * 4)
        {
            std::cerr << "Invalid RGBA pixel buffer!" << std::endl;
            return false;
        }
    }

    // Calculate mipmap levels
    int levels = 1 + (int)std::floor(
        std::log2(std::max(width, height))
    );

    if (texArray)
        glDeleteTextures(1, &texArray);

    glGenTextures(1, &texArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texArray);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexStorage3D(
        GL_TEXTURE_2D_ARRAY,
        levels,
        GL_SRGB8_ALPHA8,
        width,
        height,
        (GLsizei)imgs.size()
    );

    for (size_t i = 0; i < imgs.size(); i++)
    {
        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            0, 0, (GLint)i,
            width, height, 1,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            imgs[i].px.data()
        );
    }

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    glTexParameteri(GL_TEXTURE_2D_ARRAY,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D_ARRAY,
                    GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D_ARRAY,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D_ARRAY,
                    GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);

    glUseProgram(computeProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texArray);

    GLint loc = glGetUniformLocation(computeProgram, "textures");

    if (loc == -1)
    {
        std::cerr << "Texture uniform not found!" << std::endl;
        return false;
    }

    glUniform1i(loc, 0);

    return CheckGLError("UploadTextures");
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
		GLuint idx = glGetUniformBlockIndex(computeProgram, "Scene");
		glUniformBlockBinding(computeProgram, idx, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
	}
	else
	{
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GpuScene), &scene);
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
unsigned int OpenGLObject::getComputeShader() const {
    return computeShader;
}

void OpenGLObject::setComputeShader(unsigned int shader) {
    computeShader = shader;
}

// shaderProgram getters and setters
unsigned int OpenGLObject::getComputeProgram() const {
    return computeProgram;
}

void OpenGLObject::setComputeProgram(unsigned int program) {
    computeProgram = program;
}

// // VBO getters and setters
// unsigned int OpenGLObject::getVBO() const {
//     return VBO;
// }

// void OpenGLObject::setVBO(unsigned int vbo) {
//     VBO = vbo;
// }

// // EBO getters and setters
// unsigned int OpenGLObject::getEBO() const {
//     return EBO;
// }

// void OpenGLObject::setEBO(unsigned int ebo) {
//     EBO = ebo;
// }

// // VAO getters and setters
// unsigned int OpenGLObject::getVAO() const {
//     return VAO;
// }

// void OpenGLObject::setVAO(unsigned int vao) {
//     VAO = vao;
// }

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