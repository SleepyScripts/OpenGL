#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>

typedef struct {
	float x;
	float y;
	float z;
} Vector3;

Vector3 Add(const Vector3 *a, const Vector3 *b) {
	return (Vector3){ .x = a->x + b->x, .y = a->y + b->y, .z = a->z + b->z }; 
}

Vector3 Sub(const Vector3 *a, const Vector3 *b) {
	return (Vector3){ .x = a->x - b->x, .y = a->y - b->y, .z = a->z - b->z }; 
}

Vector3 Cross(const Vector3 *a, const Vector3 *b) {
	return (Vector3){
		.x = a->y * b->z - a->z * b->y, 
		.y = a->z * b->x - a->x * b->z, 
		.z = a->x * b->y - a->y * b->x }; 
}

float Dot(const Vector3 *a, const Vector3 *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z; 
}

float Len(const Vector3 *a) {
	return sqrt(Dot(a, a));
}

void Normalize(Vector3 *a) {
	float len = 1.0f / Len(a);
	a->x = a->x * len;
	a->y = a->y * len;
	a->z = a->z * len;
}

void Scale(Vector3 *a, float scale) {
	a->x = a->x * scale;
	a->y = a->y * scale;
	a->z = a->z * scale;
}

void PrintVector(const Vector3 *a) {
	printf("(%.3f, %.3f, %.3f)\n", a->x, a->y, a->z);
}

#endif
