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

	float *model;
} Object;

void SetDefaultModelMatrix(Object *obj);

void ObjectTranslate(Object *obj, Vector3 *position);

void ObjectScale(Object *obj, float x, float y, float z);

void ObjectRotate(Object *obj, float angle);

Object *ObjectCreate(float *vertices, size_t verticesSize, unsigned int *indices, size_t indicesSize);

void ObjectAddAttribute(Object *obj, float *uvs, size_t uvsSize);

void ObjectDelete(Object *obj);

void ObjectControl(GLFWwindow *window, Object *obj, float speed);

void ObjectRender(Object *obj, unsigned int shader, Camera *cam);

#endif
