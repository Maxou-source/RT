#include "OpenGLObject.hpp"
#include "Pattern.hpp"
#include "ShaderLoader.hpp"
#include "Texture.hpp"
#include "Light.hpp"
#include <algorithm>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glu.h>

OpenGLObject::OpenGLObject()
{
	glfwSetErrorCallback([](int code, const char* desc) {
		std::cerr << "GLFW error " << code << ": " << desc << std::endl;
	});

	// setting up graphic stuff
	if (!glfwInit())
	{
		std::cerr << "Failed to init GLFW" << std::endl;
		throw ;
	}

	// Request a window
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

	glfwGetFramebufferSize(window, &actualWidth, &actualHeight);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to init GLAD" << std::endl;
		return ;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); 

	CreateOutputTexture(actualWidth, actualHeight);

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	
	if (CheckGLError("Initialising OpenGLObkject"))
		return ;
	time.last = glfwGetTime();
}

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

	if (!CheckGLError("glCompileShader"))
		return false;
	std::cout << "Compute shader compiled successfully!" << std::endl;
	return true;
}

OpenGLObject::~OpenGLObject()
{
    if (window)
        glfwMakeContextCurrent(window);

    if (computeShader)
        glDeleteShader(computeShader);

    if (computeProgram)
        glDeleteProgram(computeProgram);

    if (debugSSBO)
        glDeleteBuffers(1, &debugSSBO);

    if (ubo)
        glDeleteBuffers(1, &ubo);

    if (texArray)
        glDeleteTextures(1, &texArray);

    if (outTex)
        glDeleteTextures(1, &outTex);

    if (readFBO)
        glDeleteFramebuffers(1, &readFBO);

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void OpenGLObject::handleResize() {
	int nw, nh;
	glfwGetFramebufferSize(window, &nw, &nh);
	if (nw == 0 || nh == 0 || (nw == actualWidth && nh == actualHeight))
		return;

	actualWidth  = nw;
	actualHeight = nh;
	CreateOutputTexture(actualWidth, actualHeight);
	UpdateCamera();
}

void OpenGLObject::UpdateCamera()
{
	cam.setViewPort(actualWidth, actualHeight);
	cam.preBuild();
	cam.packIntoGpuScene(gpuS);
	UploadCamera(gpuS);
	needsRender = true;
}

void OpenGLObject::CreateOutputTexture(int w, int h)
{
	if (outTex)
		glDeleteTextures(1, &outTex);

	glGenTextures(1, &outTex);
	glBindTexture(GL_TEXTURE_2D, outTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindImageTexture(0, outTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	if (!readFBO)
		glGenFramebuffers(1, &readFBO);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, readFBO);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
						   GL_TEXTURE_2D, outTex, 0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	
	if (glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "KO:: read framebuffer is not complete" << std::endl;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool OpenGLObject::LinkComputeProgram()
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

bool OpenGLObject::CheckGLError(const char* operation)
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

bool OpenGLObject::UploadScene(const GpuScene& scene)
{
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
	return 1;
}

void OpenGLObject::UploadCamera(const GpuScene& scene)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, offsetof(GpuScene, cam), sizeof(GpuCamera), &scene.cam);
}

bool OpenGLObject::UploadWorld(World *w) {

	// resolve textures first
	if (!w->resolveTextures())
		return false;

	cam = w->getCamera();

	gpuS = w->buildGpuScene();

	baseSpeed  = w->sceneScale() * 0.25f;
	speedMul   = 1.0f;

	if (!UploadScene(gpuS))
		return false;
	if (!UploadTextures(w->getTextures()))
		return false;
	needsRender = true;
	return true;
}


void OpenGLObject::handleMovements()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	time.now = glfwGetTime();
	float  dt  = std::min(float(time.now - time.last), 0.05f);
		time.last = time.now;
	float step = baseSpeed * speedMul * dt;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) mov.fwd += step;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) mov.fwd -= step;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) mov.rgt += step;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) mov.rgt -= step;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) mov.upw += step;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) mov.upw -= step;

	float turnSpeed = 1.5f;
	if (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS) mov.dYaw   -= turnSpeed * dt;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) mov.dYaw   += turnSpeed * dt;
	if (glfwGetKey(window, GLFW_KEY_UP)	== GLFW_PRESS) mov.dPitch += turnSpeed * dt;
	if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) mov.dPitch -= turnSpeed * dt;
}

void OpenGLObject::handleChange()
{
	changed = false;
	if (mov.fwd || mov.rgt || mov.upw) {
		cam.move(mov.fwd, mov.rgt, mov.upw);
		changed = true;
	}
	if (mov.dYaw || mov.dPitch) {
		cam.rotate(mov.dYaw, mov.dPitch);
		changed = true;
	}
	if (changed)
	{
		cam.packIntoGpuScene(gpuS);
		UploadCamera(gpuS);
		bzero(&mov, sizeof(Movements));
	}
}

void OpenGLObject::Render()
{
	if (changed || needsRender)
	{
		glUseProgram(computeProgram);
		glDispatchCompute((actualWidth + 7) / 8, (actualHeight + 7) / 8, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_FRAMEBUFFER_BARRIER_BIT);
		needsRender = false;
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, readFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, actualWidth, actualHeight, 0, 0, actualWidth, actualHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glfwSwapBuffers(window);
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

unsigned int OpenGLObject::getComputeShader() const {
	return computeShader;
}

void OpenGLObject::setComputeShader(unsigned int shader) {
	computeShader = shader;
}

unsigned int OpenGLObject::getComputeProgram() const {
	return computeProgram;
}

void OpenGLObject::setComputeProgram(unsigned int program) {
	computeProgram = program;
}

unsigned int OpenGLObject::getDebugSSBO() const {
	return debugSSBO;
}

void OpenGLObject::setDebugSSBO(unsigned int ssbo) {
	debugSSBO = ssbo;
}

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

GLuint OpenGLObject::getReadFBO() const {
	return readFBO;
}
