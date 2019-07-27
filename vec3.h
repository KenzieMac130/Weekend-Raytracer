#pragma once

typedef union
{
	struct
	{
		float x;
		float y;
		float z;
	};
	struct
	{
		float r;
		float g;
		float b;
	};
	float e[3];
} vec3;

vec3 v3Neg(vec3 a);
vec3 v3Add(vec3 a, vec3 b);
vec3 v3Addf(vec3 a, float b);
vec3 v3Sub(vec3 a, vec3 b);
vec3 v3Mult(vec3 a, vec3 b);
vec3 v3Multf(vec3 a, float b);
vec3 v3Div(vec3 a, vec3 b);
vec3 v3Divf(vec3 a, float b);

float v3Dot(vec3 a, vec3 b);
vec3 v3Cross(vec3 a, vec3 b);
vec3 v3Lerp(vec3 a, vec3 b, float t);
float v3Length(vec3 a);
float v3LengthSqr(vec3 a);
vec3 v3Unit(vec3 a);

vec3 v3RandomUnitSphere();

vec3 v3Reflect(vec3 a, vec3 b);
int v3Refract(vec3 v, vec3 n, float ni_over_nt, vec3* ref);