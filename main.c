#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <window.h>
#include <shader.h>

void WindowResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char** argv) {

	int width = 800, height = 600;
	GLFWwindow *window = CreateWindow(width, height, "OpenGL");

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "\033[1;31m%s\033[0m:%s:%s:%d\n", "ERROR", __FILE__, __func__, __LINE__);
		return -1;
	}
	
	glfwSetFramebufferSizeCallback(window, WindowResizeCallback); 


	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int indices[] = {0, 1, 2};

	unsigned int bufferVertices, bufferIndices, bufferLayout;
	glGenVertexArrays(1, &bufferLayout);
	glBindVertexArray(bufferLayout);
  
	glGenBuffers(1, &bufferVertices);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	unsigned int shaderProgram = CreateShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");

	while(!glfwWindowShouldClose(window)) {
		ProcessInput(window);

		glClearColor(0.3f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);		

		glUseProgram(shaderProgram);
		glBindVertexArray(bufferLayout);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}

	glfwTerminate();

	return 0;
}
