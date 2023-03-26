#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <window.h>
#include <shader.h>
#include <vector.h>

void WindowResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}



void SetOrthogonalProjection(unsigned int shaderProgram, 
	float left, float right, float bottom, float top, float near, float far) {
    
	float matrix[16] = {0};
    matrix[0]  = 2.0f / (right - left);
    matrix[5]  = 2.0f / (top - bottom);
    matrix[10] = -2.0f / (far - near);
    matrix[12] = -(right + left) / (right - left);
    matrix[13] = -(top + bottom) / (top - bottom);
    matrix[14] = -(far + near) / (far - near);
    matrix[15] = 1.0f;

	glUseProgram(shaderProgram);	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "proj"), 1, GL_FALSE, matrix);
}

void SetDefaultModelMatrix(unsigned int shaderProgram, float *matrix) {
	matrix[0]  = 1.0f; 
	matrix[5]  = 1.0f;
	matrix[10] = 1.0f;
	matrix[15] = 1.0f;

	glUseProgram(shaderProgram);	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, matrix);
}

void ModelTranslate(unsigned int shaderProgram, float *matrix, Vector3 *position) {
	matrix[12] = position->x;
	matrix[13] = position->y;
	matrix[14] = position->z;

	glUseProgram(shaderProgram);	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, matrix);
}

void ModelScale(unsigned int shaderProgram, float *matrix, Vector3 *scale) {
	matrix[0]  *= scale->x;
	matrix[5]  *= scale->y;
	matrix[10] *= scale->z;

	glUseProgram(shaderProgram);	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, matrix);
}

void SetDefaultViewMatrix(unsigned int shaderProgram, float *matrix) {
	matrix[0]  = 1.0f;
	matrix[5]  = 1.0f;
	matrix[10] = 1.0f;
	matrix[15] = 1.0f;

	glUseProgram(shaderProgram);	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, matrix);
}

void CameraTranslate(unsigned int shaderProgram, float *matrix, Vector3 *position) {
	matrix[12] = - position->x;
	matrix[13] = - position->y;
	matrix[14] = - position->z;

	glUseProgram(shaderProgram);	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, matrix);
}

void PlayerControl(GLFWwindow *window, unsigned int shaderProgram, float *matrix, Vector3 *position, float speed) {	
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position->y += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position->y -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position->x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position->x += speed;
	
	ModelTranslate(shaderProgram, matrix, position);
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

	float *modelMatrix = (float*)calloc(16, sizeof(float));
	float *viewMatrix  = (float*)calloc(16, sizeof(float));

	SetOrthogonalProjection(shaderProgram, 0.0f, (float)width, 0.0f, (float)height, 0.0f, 1.0f);
	SetDefaultModelMatrix(shaderProgram, modelMatrix);
	SetDefaultViewMatrix(shaderProgram, viewMatrix);
	
	Vector3 scale    = {.x = 200.0f, .y = 200.0f, .z = 1.0f};
	Vector3 position = {.x = width/2.0f, .y = height/2.0f, .z = 0.0f};

	ModelScale(shaderProgram, modelMatrix, &scale);
	
	while(!glfwWindowShouldClose(window)) {
		ProcessInput(window);	
		PlayerControl(window, shaderProgram, modelMatrix, &position, 10.0f);	


		// Rendering
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

	free(modelMatrix);
	free(viewMatrix);
	return 0;
}
