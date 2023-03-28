#ifndef CAMERA_H
#define CAMERA_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector.h>

typedef struct {
	float *proj;
	float *view;
	int width;
	int height;	
} Camera;

float *SetCameraOrthoProjectionMatrix(float left, float right, float bottom, float top, float near, float far);

float *CameraSetPerspectiveProjectionMatrix(int width, int height, float fovy, float zNear, float zFar);

float *SetDefaultCameraViewMatrix();

Camera *CameraCreate(int width, int height, float *proj);

void CameraTranslate(Camera *cam, Vector3 *position);

void CameraControl(GLFWwindow *window, Camera *cam, float speed);
	
#endif
