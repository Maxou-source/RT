#include "OpenGLObject.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

OpenGLObject::OpenGLObject()
{
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
	window = glfwCreateWindow(1080, 1920, "raytracer", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create window caca" << std::endl;
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

	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	return true;
}

bool OpenGLObject::SetFragmentShader(const char *fileName)
{

	std::ifstream shaderFile;
	
		
	shaderFile.open(filename);
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	std::string fragment = shaderStream.str();
	const char *fragmentShaderSource = fragment.c_str();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int  success;

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::frag::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
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
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 


	const int width = 1080, height = 1920;
	const int numPixels = width * height;

	
	glGenBuffers(1, &debugSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPixels * sizeof(float) * 4, nullptr, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, debugSSBO); // binding = 0, matches shader
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}


/*============= SETTERS && GETTERS ===========*/


const char* OpenGLObject::getInfoLog() const {
    return infoLog;
}

void OpenGLObject::setInfoLog(const char* log) {
    if (log != nullptr) {
        strncpy(infoLog, log, sizeof(infoLog) - 1);
        infoLog[sizeof(infoLog) - 1] = '\0'; // Ensure null termination
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

