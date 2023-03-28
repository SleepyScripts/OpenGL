#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <window.h>
#include <shader.h>
#include <vector.h>
#include <object.h>
#include <camera.h>

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
		  0.5f,  0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f
	};

	unsigned int indices[] = {0, 1, 2, 0, 2, 3};
	
	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	unsigned int shader = ShaderCreate("shaders/vertex.shader", "shaders/fragment.shader");
	
	float *proj = SetCameraOrthoProjectionMatrix(0.0f, (float)width, 0.0f, (float)height, 0.0f, 1.0f);
	Camera *cam = CameraCreate(width, height, proj);
	
	Object *obj = ObjectCreate(vertices, sizeof(vertices), indices, sizeof(indices)); 
	ObjectAddAttribute(obj, uvs, sizeof(uvs));
	ObjectScale(obj, 200.0f, 200.0f, 1.0f);
	
	Object *obj1 = ObjectCreate(vertices, sizeof(vertices), indices, sizeof(indices)); 
	ObjectScale(obj1, 150.0f, 150.0f, 1.0f);
	
	Object *obj2 = ObjectCreate(vertices, sizeof(vertices), indices, sizeof(indices)); 
	ObjectAddAttribute(obj2, uvs, sizeof(uvs));
	ObjectScale(obj2, 50.0f, 50.0f, 1.0f);
	
	while(!glfwWindowShouldClose(window)) {
		ProcessInput(window);
			
		ObjectControl(window, obj, 10.0f);	
		CameraControl(window, cam, 5.0f);
		
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);		

		ObjectRender(obj,  shader, cam);
		ObjectRender(obj1, shader, cam);
		ObjectRender(obj2, shader, cam);

    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}

	glfwTerminate();
	ObjectDelete(obj);
	ObjectDelete(obj1);
	ObjectDelete(obj2);

	return 0;
}
