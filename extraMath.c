#include "extraMath.h"

#include <stdlib.h>


float lerpf(float a, float b, float t)
{
	return (1.0f - t) * a + t * b;
}

float floatRandom()
{
	return (float)(rand()) / RAND_MAX;
}

float fresnel_schlick(float cosine, float ior)
{
	float r0 = (1.0f - ior) / (1.0f + ior);
	r0 = r0 * r0;
	return r0 + (1 - r0)*powf((1.0f - cosine), 5.0f);
}

float unchartedTonemap(float x)
{
	const float A = 0.15;
	const float B = 0.5f;
	const float C = 0.1f;
	const float D = 0.2f;
	const float E = 0.02f;
	const float F = 0.3f;
	const float W = 11.2f;
	return ((x*(A*x + C * B) + D * E) / (x*(A*x + B) + D * F)) - E / F;
};
