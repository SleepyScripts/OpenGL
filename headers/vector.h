#ifndef VECTOR_H
#define VECTOR_H
#include <stdio.h>
#include <math.h>

typedef struct {
	float x;
	float y;
	float z;
} Vector3;

Vector3 Add(const Vector3 *a, const Vector3 *b);

Vector3 Sub(const Vector3 *a, const Vector3 *b);

Vector3 Cross(const Vector3 *a, const Vector3 *b);

float Dot(const Vector3 *a, const Vector3 *b);

float Len(const Vector3 *a);

void Normalize(Vector3 *a);

void Scale(Vector3 *a, float scale);

void PrintVector(const Vector3 *a);

void MatrixMul(float *out, float *m1, float *m2);

#endif
