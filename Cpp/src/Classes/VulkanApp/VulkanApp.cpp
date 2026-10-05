#include "VulkanApp.h"
void VulkanApp::run() {
	initVulkan();
	mainLoop();
	cleanup();
}

void VulkanApp::initVulkan() {

}
void VulkanApp::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Vulkan", nullptr, nullptr);
}
void VulkanApp::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}
void VulkanApp::mainLoop() {
	// simply loops and checks for events
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}
