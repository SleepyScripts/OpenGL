#include <stdio.h>
#include <math.h>
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	float *vertices;
	float *normals;
	float *uvs;
	unsigned int *indices;
	
	unsigned int numVertices;
	unsigned int numTriangles;
} Mesh;


typedef struct {
	vec3 position;
	vec3 rotation;
	vec3 scale;
	mat4 model;

	Mesh *meshes;
	unsigned int numMeshes;
	unsigned int shader;
	unsigned int *layout;
} Object;

void ObjectImportModel(Object *obj, const char *name) {
	const struct aiScene* scene = aiImportFile(name, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene) {
	    printf("Error loading model: %s", aiGetErrorString());
	    exit(1);
	}

	obj->numMeshes = scene->mNumMeshes;
	obj->meshes = malloc(sizeof(Mesh) * obj->numMeshes);
	for(int currentMesh = 0; currentMesh < obj->numMeshes; currentMesh++) {
		Mesh *mesh = malloc(sizeof(Mesh));
		
		mesh->numVertices  = scene->mMeshes[currentMesh]->mNumVertices;
		mesh->numTriangles = scene->mMeshes[currentMesh]->mNumFaces;
		
		mesh->vertices = malloc(sizeof(float) * 3 * mesh->numVertices);
		mesh->normals  = malloc(sizeof(float) * 3 * mesh->numVertices);
		mesh->uvs      = malloc(sizeof(float) * 2 * mesh->numVertices);
		mesh->indices  = malloc(sizeof(unsigned int) * 3 * mesh->numTriangles);


		memcpy(mesh->vertices, scene->mMeshes[currentMesh]->mVertices,          sizeof(float) * 3 * mesh->numVertices);
		memcpy(mesh->normals,  scene->mMeshes[currentMesh]->mNormals,           sizeof(float) * 3 * mesh->numVertices);
		memcpy(mesh->uvs,      scene->mMeshes[currentMesh]->mFaces[0].mIndices, sizeof(float) * 2 * mesh->numVertices);

		for (unsigned int i = 0; i < scene->mMeshes[currentMesh]->mNumFaces; i++) {
	    	struct aiFace face = scene->mMeshes[currentMesh]->mFaces[i];
			mesh->indices[i*3]   = face.mIndices[0];
			mesh->indices[i*3+1] = face.mIndices[1];
			mesh->indices[i*3+2] = face.mIndices[2];
		}

		obj->meshes[currentMesh] = *mesh;
	}

	aiReleaseImport(scene);
}

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

void ObjectDelete(Object *obj) {
	for(int i=0; i<obj->numMeshes; i++) {
		free(obj->meshes[i].vertices);
		free(obj->meshes[i].indices);
		free(obj->meshes[i].normals);
		free(obj->meshes[i].uvs);
	}
	free(obj->meshes);
}

void ObjectRender(Object *obj, Camera *camera) {
	glUseProgram(obj->shader);
	glUniformMatrix4fv(glGetUniformLocation(obj->shader, "model"), 1, GL_FALSE, obj->model[0]);
	glUniformMatrix4fv(glGetUniformLocation(obj->shader, "view" ), 1, GL_FALSE, camera->view[0]);
	glUniformMatrix4fv(glGetUniformLocation(obj->shader, "proj" ), 1, GL_FALSE, camera->proj[0]);

	for(int m=0; m<obj->numMeshes; m++) {	
		glBindVertexArray(obj->layout[m]);
		glDrawElements(GL_TRIANGLES, obj->meshes[m].numTriangles * 3, GL_UNSIGNED_INT, 0);
	}
}

void ObjectSetLayout(Object *obj) {
	obj->layout = malloc(sizeof(unsigned int) * obj->numMeshes);
	
	for(int m = 0; m < obj->numMeshes; m++) {
		unsigned int bufferVertices, bufferIndices, bufferNormals, bufferUvs; 
		glGenVertexArrays(1, &(obj->layout[m]));
		glBindVertexArray(obj->layout[m]);

		glGenBuffers(1, &bufferVertices);
		glBindBuffer(GL_ARRAY_BUFFER, bufferVertices);
		glBufferData(GL_ARRAY_BUFFER, obj->meshes[m].numVertices * 3 * sizeof(float), obj->meshes[m].vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &bufferNormals);
		glBindBuffer(GL_ARRAY_BUFFER, bufferNormals);
		glBufferData(GL_ARRAY_BUFFER, obj->meshes[m].numVertices * 3 * sizeof(float), obj->meshes[m].normals, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
	
		glGenBuffers(1, &bufferUvs);
		glBindBuffer(GL_ARRAY_BUFFER, bufferUvs);
		glBufferData(GL_ARRAY_BUFFER, obj->meshes[m].numVertices * 2 * sizeof(float), obj->meshes[m].uvs, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);


		glGenBuffers(1, &bufferIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->meshes[m].numTriangles * 3 * sizeof(unsigned int), obj->meshes[m].indices, GL_STATIC_DRAW); 

		glBindVertexArray(0);
	}
}
	
int main(int argc, char** argv) {	
	int width = 800, height = 600;
	GLFWwindow *window = CreateWindow(width, height, "OpenGL");
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "\033[1;31m%s\033[0m:%s:%s:%d\n", "ERROR", __FILE__, __func__, __LINE__);
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, WindowResizeCallback); 
	

	vec3 pos = {0.0f, 0.0f, -2.0f};
	Camera camera; 
	CameraCreate(&camera, pos, width, height, 0);	
	vec3 position = {0.0f, 0.0f, 0.0f};
	vec3 rotation = {-90.0f, 0.0f, 0.0f};
	vec3 scale = {10.0f, 10.0f, 10.0f};
	
	Object obj;
	ObjectCreate(&obj, position, rotation, scale);
 	ObjectGetModelMatrix(&obj);
	ObjectImportModel(&obj, "models/dragon.obj");
	obj.shader =  ShaderCreate("shaders/vertex.shader", "shaders/fragment.shader");	
 	ObjectSetLayout(&obj);

	
	glEnable(GL_DEPTH_TEST);	
	vec3 X = {1.0f, 0.0f, 0.0f};
	vec3 Y = {0.0f, 1.0f, 0.0f};
	vec3 Z = {0.0f, 0.0f, 1.0f};
	while(!glfwWindowShouldClose(window)) {
		ProcessInput(window);				
		CameraControl(window, &camera, 5.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		
		glm_rotate(obj.model, glm_rad(1.0f), Z); 
		ObjectRender(&obj, &camera);
	
    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}

	glfwTerminate();
	ObjectDelete(&obj);
	return 0;
}
