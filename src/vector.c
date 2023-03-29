#include <vector.h>

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

void MatrixMul(float *out, float *m1, float *m2) {
	out[0]= m1[0]*m2[0]+m1[1]*m2[4]+m1[2]*m2[8]+ m1[3]*m2[12]; 
	out[1]= m1[0]*m2[1]+m1[1]*m2[5]+m1[2]*m2[9]+ m1[3]*m2[13]; 
	out[2]= m1[0]*m2[2]+m1[1]*m2[6]+m1[2]*m2[10]+m1[3]*m2[14]; 
	out[3]= m1[0]*m2[3]+m1[1]*m2[7]+m1[2]*m2[11]+m1[3]*m2[15];

	out[4]= m1[4]*m2[0]+m1[5]*m2[4]+m1[6]*m2[8]+ m1[7]*m2[12]; 
	out[5]= m1[4]*m2[1]+m1[5]*m2[5]+m1[6]*m2[9]+ m1[7]*m2[13]; 
	out[6]= m1[4]*m2[2]+m1[5]*m2[6]+m1[6]*m2[10]+m1[7]*m2[14]; 
	out[7]= m1[4]*m2[3]+m1[5]*m2[7]+m1[6]*m2[11]+m1[7]*m2[15];
	
	out[8] = m1[8]*m2[0]+m1[9]*m2[4]+m1[10]*m2[8]+ m1[11]*m2[12]; 
	out[9] = m1[8]*m2[1]+m1[9]*m2[5]+m1[10]*m2[9]+ m1[11]*m2[13]; 
	out[10]= m1[8]*m2[2]+m1[9]*m2[6]+m1[10]*m2[10]+m1[11]*m2[14]; 
	out[11]= m1[8]*m2[3]+m1[9]*m2[7]+m1[10]*m2[11]+m1[11]*m2[15];

	out[12]= m1[12]*m2[0]+m1[13]*m2[4]+m1[14]*m2[8]+ m1[15]*m2[12]; 
	out[13]= m1[12]*m2[1]+m1[13]*m2[5]+m1[14]*m2[9]+ m1[15]*m2[13]; 
	out[14]= m1[12]*m2[2]+m1[13]*m2[6]+m1[14]*m2[10]+m1[15]*m2[14]; 
	out[15]= m1[12]*m2[3]+m1[13]*m2[7]+m1[14]*m2[11]+m1[15]*m2[15];	
}

