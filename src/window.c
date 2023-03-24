#include <window.h>

GLFWwindow *CreateWindow(int width, int height, const char *name) {
	if(glfwInit() == GLFW_FALSE) {
		fprintf(stderr, "\033[1:31m%s\033[0m:%s:%s:%d\n", "ERROR", __FILE__, __func__, __LINE__);
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	if(window == NULL) {
		fprintf(stderr, "\033[1;31m%s\033[0m:%s:%s:%d\n", "ERROR", __FILE__, __func__, __LINE__);
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	

	return window;	
}

