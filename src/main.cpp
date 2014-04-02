#include <string>
#include <sstream>

#include "OpenGL.h"
#include "GLM.h"

#include "Util/logging.h"
#include "Util/glerror.h"

GLFWwindow* window;
GLint glVersion[2];
GLchar* glVendor;

int main(int argc, char* argv[])
{
	// Initialize GLFW
	if (!glfwInit())
	{
		LOG_ERROR("GLFW: Initialization failed");
		return 1;
	}

	// Create a window
	window = glfwCreateWindow(1280, 720, "OpenGL", nullptr, nullptr);
	if (!window)
	{
		LOG_ERROR("GLFW: Failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	// GL version info
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
	glVendor = (GLchar*)glGetString(GL_VENDOR);
	std::stringstream ss;
	ss << glVendor << " OpenGL " << glVersion[0] << "." << glVersion[1];
#ifdef DEBUG
	ss << " DEBUG";
#endif
	LOG_INFO(ss.str().c_str());
	glfwSetWindowTitle(window, ss.str().c_str());

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		LOG_ERROR("GLEW: Initialization failed");
		return 1;
	}

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	return 0;
}