#include "gl/shared/gltools.h"	// OpenGL toolkit
#include <math.h>

#ifndef VECTOR_CALCULATE
#define VECTOR_CALCULATE

float clamp(float x, float a, float b){ return x<a ? a : (x<b ? x : b); };
float dot(const float *a, const float *b){ return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; };
float norm(const float *a){ return sqrtf(dot(a, a)); };
void vassign(float *a, float x, float y, float z){ a[0] = x; a[1] = y; a[2] = z; };
void vassign(float *a, const float *b){ a[0] = b[0]; a[1] = b[1]; a[2] = b[2]; };
void cross(float *a, const float *b, const float *c)
{
	a[0] = b[1] * c[2] - c[1] * b[2];
	a[1] = -b[0] * c[2] + c[0] * b[2];
	a[2] = b[0] * c[1] - c[0] * b[1];
};
void normalize(float *a)
{
	float l = norm(a);
	a[0] /= l; a[1] /= l; a[2] /= l;
};
void PlusAndNorm(float *a, float *b, float *c){
	a[0] = b[0] + c[0];
	a[1] = b[1] + c[1];
	a[2] = b[2] + c[2];
	normalize(a);
};
void MinusAndNorm(float *a, float *b, float *c){
	a[0] = b[0] - c[0];
	a[1] = b[1] - c[1];
	a[2] = b[2] - c[2];
	normalize(a);
};
void vectorscale(float d, float *a){ a[0] = a[0] * d; a[1] = a[1] * d; a[2] = a[2] * d; }
void getPointbyVec(float *np, float *p, float *v){ np[0] = p[0] + v[0]; np[1] = p[1] + v[1]; np[2] = p[2] + v[2]; };
void verplot(float *a){ glVertex3f(a[0], a[1], a[2]); }
#endif