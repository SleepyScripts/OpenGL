#include <stdio.h>
#include <math.h>
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <window.h>
#include <shader.h>
#include <camera.h>

void WindowResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

typedef struct {
	vec3 position;
	vec3 rotation;
	vec3 scale;

	mat4 model;
} Object;

void ObjectCreate(Object *obj, vec3 position, vec3 rotation, vec3 scale) {
	obj->position[0] = position[0];
	obj->position[1] = position[1];
	obj->position[2] = position[2];
	
	obj->rotation[0] = rotation[0];
	obj->rotation[1] = rotation[1];
	obj->rotation[2] = rotation[2];
	
	obj->scale[0] = scale[0];
	obj->scale[1] = scale[1];
	obj->scale[2] = scale[2];
}

void ObjectGetModelMatrix(Object *obj) {
	glm_mat4_identity(obj->model);

	vec3 X = {1.0f, 0.0f, 0.0f};
	vec3 Y = {0.0f, 1.0f, 0.0f};
	vec3 Z = {0.0f, 0.0f, 1.0f};

	glm_rotate(obj->model, glm_rad(obj->rotation[0]), X); 
	glm_rotate(obj->model, glm_rad(obj->rotation[1]), Y); 
	glm_rotate(obj->model, glm_rad(obj->rotation[2]), Z); 
	
	mat4 scalingMatrix = GLM_MAT4_IDENTITY_INIT;
	glm_scale(scalingMatrix, obj->scale);

	glm_mat4_mul(scalingMatrix, obj->model, obj->model);

	obj->model[3][0] = obj->position[0];
	obj->model[3][1] = obj->position[1];
	obj->model[3][2] = obj->position[2];	
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
		 -0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f, -0.5f,
		  0.5f,  0.5f, -0.5f,
		 -0.5f,  0.5f, -0.5f,
		
		 -0.5f, -0.5f, 0.5f,
		  0.5f, -0.5f, 0.5f,
		  0.5f,  0.5f, 0.5f,
		 -0.5f,  0.5f, 0.5f
	};

	unsigned int indices[] = {
		0, 1, 2, 
		0, 2, 3,	
		4, 5, 6, 
		4, 6, 7	
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	
	unsigned int shader = ShaderCreate("shaders/vertex.shader", "shaders/fragment.shader");

	vec3 pos = {0.0f, 0.0f, -2.0f};
	Camera camera; 
	CameraCreate(&camera, pos, width, height, 0);	
	vec3 position = {0.0f, 0.0f, 0.0f};
	vec3 rotation = {0.0f, 0.0f, 45.0f};
	vec3 scale = {100.0f, 100.0f,100.0f};
	Object obj;
	ObjectCreate(&obj, position, rotation, scale);
 	ObjectGetModelMatrix(&obj);
	
 	unsigned int bufferLayout, bufferVertices, bufferIndices, bufferUvs; 
	glGenVertexArrays(1, &bufferLayout);
	glBindVertexArray(bufferLayout);

	glGenBuffers(1, &bufferVertices);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &bufferUvs);
	glBindBuffer(GL_ARRAY_BUFFER, bufferUvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	glBindVertexArray(0);
		
	glEnable(GL_DEPTH_TEST);	
	
	while(!glfwWindowShouldClose(window)) {
		ProcessInput(window);				
		CameraControl(window, &camera, 5.0f);

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		
		glm_rotate(obj.model, glm_rad(1.0f), GLM_YUP); 


		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, obj.model[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "view" ), 1, GL_FALSE, camera.view[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "proj" ), 1, GL_FALSE, camera.proj[0]);
	
		glBindVertexArray(bufferLayout);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);


    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}

	glfwTerminate();

	return 0;
}
