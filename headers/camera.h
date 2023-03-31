#ifndef CAMERA_H
#define CAMERA_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

typedef struct{
	mat4 proj;
	mat4 view;
} Camera;

void CameraCreate(Camera *cam, vec3 pos, int width, int height, int type);

void CameraTranslate(Camera *cam, vec3 pos);

void CameraControl(GLFWwindow *window, Camera *cam, float speed);

#endif
