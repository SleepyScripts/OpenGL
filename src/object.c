#include <object.h>

Object *ObjectCreate(float *vertices, size_t verticesSize, unsigned int *indices, size_t indicesSize) {
	Object *obj = malloc(sizeof(Object));
	
	obj->countAttribute = 0;
	obj->countIndices = indicesSize/sizeof(unsigned int);	
	
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
	free(obj);
}

void ObjectSetPosition(Object *obj, float x, float y, float z) {
	obj->position.x = x;
	obj->position.y = y;
	obj->position.z = z;
}

void ObjectSetScale(Object *obj, float x, float y, float z) {
	obj->scale.x = x;
	obj->scale.y = y;
	obj->scale.z = z;
}

void ObjectSetRotation(Object *obj, float x, float y, float z) {
	obj->rotation.x = x;
	obj->rotation.y = y;
	obj->rotation.z = z;
}

void ObjectControl(GLFWwindow *window, Object *obj, float speed) {	
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
       	obj->position.y += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        obj->position.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        obj->position.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        obj->position.x += speed;
}

void ObjectRender(Object *obj, unsigned int shader, Camera *cam) {

	float cX = cos(obj->rotation.x);
    float sX = sin(obj->rotation.x);
	float cY = cos(obj->rotation.y);
    float sY = sin(obj->rotation.y);
	float cZ = cos(obj->rotation.z);
    float sZ = sin(obj->rotation.z);
	
	float scale[16] = {obj->scale.x,0,0,0,0,obj->scale.y,0,0,0,0,obj->scale.z,0,0,0,0,1.0f};	
	
	float rotationX[16] = {1.0f,0,0,0, 0,cX,sX,0,0,-sX,cX,0,0,0,0,1.0f};
	float rotationY[16]  = {cY,0,-sY,0,0,1.0f,0,0,sY,0,cY,0,0,0,0,1.0f};
	float rotationZ[16]  = {cZ,sZ,0,0,-sZ,cZ,0,0,0,0,1.0f,0,0,0,0,1.0f};
	float rotationXY[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	float rotation[16]   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	MatrixMul(rotationXY, rotationX, rotationY);
	MatrixMul(rotation, rotationXY, rotationZ);
	
	float model[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	MatrixMul(model, scale, rotation);
	model[12] = obj->position.x;
	model[13] = obj->position.y;
	model[14] = obj->position.z;

	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view" ), 1, GL_FALSE, cam->view);
	glUniformMatrix4fv(glGetUniformLocation(shader, "proj" ), 1, GL_FALSE, cam->proj);
	
	glBindVertexArray(obj->bufferLayout);
	glDrawElements(GL_TRIANGLES, obj->countIndices, GL_UNSIGNED_INT, 0);
}
