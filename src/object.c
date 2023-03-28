#include <object.h>

void SetDefaultModelMatrix(Object *obj) {
	obj->model = calloc(16, sizeof(float));
	obj->model[0]  = 1.0f; 
	obj->model[5]  = 1.0f;
	obj->model[10] = 1.0f;
	obj->model[15] = 1.0f;
}

void ObjectTranslate(Object *obj, Vector3 *position) {
	obj->model[12] = position->x;
	obj->model[13] = position->y;
	obj->model[14] = position->z;
}

void ObjectScale(Object *obj, float x, float y, float z) {
	obj->model[0]  *= x;
	obj->model[5]  *= y;
	obj->model[10] *= z;
}

void ObjectRotate(Object *obj, float angle) {
	float c = cos(angle);
    float s = sin(angle);
    obj->model[0]  = 200.0f * c;
    obj->model[2]  = -s * 200.0f;
    obj->model[5]  = 200.0f;
    obj->model[8]  = s * 200.0f;
    obj->model[10] = c * 200.0f;
}

Object *ObjectCreate(float *vertices, size_t verticesSize, unsigned int *indices, size_t indicesSize) {
	Object *obj = malloc(sizeof(Object));
	obj->countAttribute = 0;
	obj->countIndices = indicesSize/sizeof(unsigned int);	
	SetDefaultModelMatrix(obj);
	
	glGenVertexArrays(1, &obj->bufferLayout);
	glBindVertexArray(obj->bufferLayout);

 	unsigned int bufferVertices, bufferIndices; 
	glGenBuffers(1, &bufferVertices);
	glBindBuffer(GL_ARRAY_BUFFER, bufferVertices);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(obj->countAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(obj->countAttribute);

	glGenBuffers(1, &bufferIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW); 

	glBindVertexArray(0);
	return obj;
}

void ObjectAddAttribute(Object *obj, float *uvs, size_t uvsSize) {
	obj->countAttribute += 1;
	glBindVertexArray(obj->bufferLayout);
	unsigned int bufferUvs;
	glGenBuffers(1, &bufferUvs);
	glBindBuffer(GL_ARRAY_BUFFER, bufferUvs);
	glBufferData(GL_ARRAY_BUFFER, uvsSize, uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(obj->countAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(obj->countAttribute);
	glBindVertexArray(0);
}

void ObjectDelete(Object *obj) {
	free(obj->model);
	free(obj);
}

void ObjectControl(GLFWwindow *window, Object *obj, float speed) {	
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
       	obj->model[13] += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        obj->model[13] -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        obj->model[12] -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        obj->model[12] += speed;
}

void ObjectRender(Object *obj, unsigned int shader, Camera *cam) {
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, obj->model);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view" ), 1, GL_FALSE, cam->view);
	glUniformMatrix4fv(glGetUniformLocation(shader, "proj" ), 1, GL_FALSE, cam->proj);
	
	glBindVertexArray(obj->bufferLayout);
	glDrawElements(GL_TRIANGLES, obj->countIndices, GL_UNSIGNED_INT, 0);
}
