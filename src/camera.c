#include <camera.h>

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

float *CameraSetPerspectiveProjectionMatrix(int width, int height, float fovy, float zNear, float zFar) {
	float *matrix = calloc(16, sizeof(float));
	float aspect = (float)width / (float)height;
    float f = 1.0f / tanf(fovy * (float)M_PI / 360.0f);
    matrix[0] = f / aspect;
    matrix[5] = f;
    matrix[10] = (zFar + zNear) / (zNear - zFar);
    matrix[11] = -1.0f;
    matrix[14] = (2.0f * zFar * zNear) / (zNear - zFar);
	return matrix;
}

float *SetDefaultCameraViewMatrix() {
	float *view = calloc(16, sizeof(float));
	view[0]  = 1.0f; 
	view[5]  = 1.0f;
	view[10] = 1.0f;
	view[15] = 1.0f;
	return view;
}

Camera *CameraCreate(int width, int height, float *proj) {
	Camera *cam = malloc(sizeof(Camera));
	cam->width  = width;
	cam->height = height;
	cam->proj   = proj;
	cam->view   = SetDefaultCameraViewMatrix();
	return cam;
}

void CameraTranslate(Camera *cam, Vector3 *position) {
	cam->view[12] = - position->x;
	cam->view[13] = - position->y;
	cam->view[14] = - position->z;
}

void CameraControl(GLFWwindow *window, Camera *cam, float speed) {	
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        cam->view[13] += speed;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        cam->view[13] -= speed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        cam->view[12] -= speed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        cam->view[12] += speed;	
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        cam->view[14] -= speed;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        cam->view[14] += speed;	
}

