#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <window.h>
#include <shader.h>
#include <vector.h>

typedef struct {
	unsigned int shaderProgram;
	unsigned int bufferVertices;
	unsigned int bufferIndices;
	unsigned int bufferLayout;
	unsigned int attributeCounter;

	float *model;
} Object;

typedef struct {
	float *proj;
	float *view;
	int width;
	int height;	
} Camera;


void SetDefaultModelMatrix(Object *obj) {
	obj->model = calloc(16, sizeof(float));
	obj->model[0]  = 1.0f; 
	obj->model[5]  = 1.0f;
	obj->model[10] = 1.0f;
	obj->model[15] = 1.0f;

	glUseProgram(obj->shaderProgram);	
	glUniformMatrix4fv(glGetUniformLocation(obj->shaderProgram, "model"), 1, GL_FALSE, obj->model);
}

void ObjectTranslate(Object *obj, Vector3 *position) {
	obj->model[12] = position->x;
	obj->model[13] = position->y;
	obj->model[14] = position->z;

	glUseProgram(obj->shaderProgram);	
	glUniformMatrix4fv(glGetUniformLocation(obj->shaderProgram, "model"), 1, GL_FALSE, obj->model);
}

void ObjectScale(Object *obj, Vector3 *scale) {
	obj->model[0]  *= scale->x;
	obj->model[5]  *= scale->y;
	obj->model[10] *= scale->z;

	glUseProgram(obj->shaderProgram);	
	glUniformMatrix4fv(glGetUniformLocation(obj->shaderProgram, "model"), 1, GL_FALSE, obj->model);
}

Object *CreateObject(Camera *cam, float *vertices, size_t verticesSize, unsigned int *indices, size_t indicesSize, unsigned int shaderProgram) {
	Object *obj = malloc(sizeof(Object));
	obj->attributeCounter = 0;
	obj->shaderProgram = shaderProgram;
	
	SetDefaultModelMatrix(obj);

	glUniformMatrix4fv(glGetUniformLocation(obj->shaderProgram, "view"), 1, GL_FALSE, cam->view);
	glUniformMatrix4fv(glGetUniformLocation(obj->shaderProgram, "proj"), 1, GL_FALSE, cam->proj);
	
	glGenVertexArrays(1, &obj->bufferLayout);
	glBindVertexArray(obj->bufferLayout);
  
	glGenBuffers(1, &obj->bufferVertices);
	glBindBuffer(GL_ARRAY_BUFFER, obj->bufferVertices);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(obj->attributeCounter, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(obj->attributeCounter);

	glGenBuffers(1, &obj->bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW); 

	glBindVertexArray(0);
	return obj;
}

void DeleteObject(Object *obj) {
	free(obj->model);
	free(obj);
}

void ObjectControl(GLFWwindow *window, Object *obj, Vector3 *position, float speed) {	
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position->y += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position->y -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position->x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position->x += speed;
	
	ObjectTranslate(obj, position);
}







typedef struct {
	Object **array;
	unsigned int objectCount;
} ObjectPool;

ObjectPool *CreateObjectPool(unsigned int count) {
	ObjectPool *objectPool = malloc(sizeof(ObjectPool));
	objectPool->objectCount = 0;
	objectPool->array = malloc(sizeof(Object) * count);
	
	return objectPool;
}

void DeleteObjectPool(ObjectPool *objectPool) {
	free(objectPool->array);
	free(objectPool);
}

void AddObject(ObjectPool *objectPool, Object *obj) {
	objectPool->array[objectPool->objectCount] = obj;
	objectPool->objectCount += 1; 
}





float *SetCameraOrthoProjectionMatrix(float left, float right, float bottom, float top, float near, float far) {
	float *proj = calloc(16, sizeof(float));
	proj[0]  = 2.0f / (right - left);
    proj[5]  = 2.0f / (top - bottom);
    proj[10] = -2.0f / (far - near);
    proj[12] = -(right + left) / (right - left);
    proj[13] = -(top + bottom) / (top - bottom);
    proj[14] = -(far + near) / (far - near);
    proj[15] = 1.0f;
	return proj;
}

float *SetDefaultCameraViewMatrix() {
	float *view = calloc(16, sizeof(float));
	view[0]  = 1.0f; 
	view[5]  = 1.0f;
	view[10] = 1.0f;
	view[15] = 1.0f;
	return view;
}

Camera *CreateCamera(int width, int height, float *proj) {
	Camera *cam = malloc(sizeof(Camera));
	cam->width  = width;
	cam->height = height;
	cam->proj   = proj;
	cam->view   = SetDefaultCameraViewMatrix();
	return cam;
}


void CameraTranslate(Camera *cam, Vector3 *position, ObjectPool *objectPool) {
	cam->view[12] = - position->x;
	cam->view[13] = - position->y;
	cam->view[14] = - position->z;

	for(int i=0; i<objectPool->objectCount; i++) {
		glUseProgram(objectPool->array[i]->shaderProgram);	
		glUniformMatrix4fv(glGetUniformLocation(objectPool->array[i]->shaderProgram, "view"), 1, GL_FALSE, cam->view);
	}
}


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

	unsigned int shaderProgram = CreateShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");
	
	float *proj = SetCameraOrthoProjectionMatrix(0.0f, (float)width, 0.0f, (float)height, 0.0f, 1.0f);
	Camera *cam = CreateCamera(width, height, proj);
	
	Object *obj = CreateObject(cam, vertices, sizeof(vertices), indices, sizeof(indices), shaderProgram); 
	Vector3 scale    = {.x = 200.0f, .y = 200.0f, .z = 1.0f};
	Vector3 position = {.x = width/2.0f, .y = height/2.0f, .z = 0.0f};
	ObjectScale(obj, &scale);
	
	ObjectPool *objectPool = CreateObjectPool(2);
	AddObject(objectPool, obj);

	
	while(!glfwWindowShouldClose(window)) {
		ProcessInput(window);	
		ObjectControl(window, obj, &position, 10.0f);	

		// Rendering
		glClearColor(0.3f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);		

		glUseProgram(obj->shaderProgram);
		glBindVertexArray(obj->bufferLayout);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}

	glfwTerminate();
	DeleteObject(obj);
	DeleteObjectPool(objectPool);

	return 0;
}
