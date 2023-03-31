#include <camera.h>

void CameraCreate(Camera *cam, vec3 pos, int width, int height, int type) {
	glm_mat4_identity(cam->view);
	glm_mat4_identity(cam->proj);

	float fovy = 45.0f;
    float aspect = (float)width / (float)height;
    float near = 0.1f; 
    float far = 1000.0f;  

	if(type == 0) {
    	glm_perspective(fovy, aspect, near, far, cam->proj);	
	} else if(type == 1) {
		glm_ortho(0.0f, (float)width, 0.0f, (float)height, near, far, cam->proj);
	}
	cam->view[3][0] = pos[0]; 
	cam->view[3][1] = pos[1]; 
	cam->view[3][2] = pos[2]; 
}

void CameraTranslate(Camera *cam, vec3 pos) {
	cam->view[3][0] = pos[0];
	cam->view[3][1] = pos[1];
	cam->view[3][2] = pos[2];
}

void CameraControl(GLFWwindow *window, Camera *cam, float speed) {	
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        cam->view[3][1] += speed;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        cam->view[3][1] -= speed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        cam->view[3][0] -= speed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        cam->view[3][0] += speed;	
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        cam->view[3][2] -= speed;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        cam->view[3][2] += speed;	
}

