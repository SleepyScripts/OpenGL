#ifndef OBJECT_H
#define OBJECT_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector.h>
#include <camera.h>

typedef struct {
	unsigned int bufferLayout;
	unsigned int countAttribute;
	unsigned int countIndices;

	Vector3 position;
	Vector3 scale;
	Vector3 rotation;
} Object;

Object *ObjectCreate(float *vertices, size_t verticesSize, unsigned int *indices, size_t indicesSize);

void ObjectAddAttribute(Object *obj, float *uvs, size_t uvsSize);

void ObjectDelete(Object *obj);

void ObjectSetPosition(Object *obj, float x, float y, float z);

void ObjectSetScale(Object *obj, float x, float y, float z);

void ObjectSetRotation(Object *obj, float x, float y, float z);

void ObjectControl(GLFWwindow *window, Object *obj, float speed);

void ObjectRender(Object *obj, unsigned int shader, Camera *cam);

#endif
