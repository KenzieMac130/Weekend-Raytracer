#include "vec3.h"

#include <math.h>
#include "extraMath.h"

vec3 v3Neg(vec3 a)
{
	vec3 r = { -a.x,-a.y,-a.z };
	return r;
}

vec3 v3Add(vec3 a, vec3 b)
{
	vec3 r = { a.x + b.x ,a.y + b.y,a.z + b.z };
	return r;
}

vec3 v3Addf(vec3 a, float b)
{
	vec3 r = { a.x + b ,a.y + b,a.z + b };
	return r;
}

vec3 v3Sub(vec3 a, vec3 b)
{
	vec3 r = { a.x - b.x ,a.y - b.y,a.z - b.z };
	return r;
}

vec3 v3Mult(vec3 a, vec3 b)
{
	vec3 r = { a.x * b.x ,a.y * b.y,a.z * b.z };
	return r;
}

vec3 v3Multf(vec3 a, float b)
{
	vec3 r = { a.x * b ,a.y * b,a.z * b };
	return r;
}

vec3 v3Div(vec3 a, vec3 b)
{
	vec3 r = { a.x / b.x ,a.y / b.y,a.z / b.z };
	return r;
}

vec3 v3Divf(vec3 a, float b)
{
	vec3 r = { a.x / b ,a.y / b,a.z / b };
	return r;
}

float v3Dot(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 v3Cross(vec3 a, vec3 b)
{
	vec3 r ={
	(a.e[1] * b.e[2] - a.e[2] * b.e[1]),
	(-(a.e[0] * b.e[2] - a.e[2] * b.e[0])),
	(a.e[0] * b.e[1] - a.e[1] * b.e[0]) };
	return r;
}

vec3 v3Lerp(vec3 a, vec3 b, float t)
{
	vec3 r =
	{ lerpf(a.r, b.r, t),
	lerpf(a.g, b.g, t),
	lerpf(a.b, b.b, t) };
	return r;
}

float v3Length(vec3 a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

float v3LengthSqr(vec3 a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z;
}

vec3 v3Unit(vec3 a)
{
	return v3Divf(a, v3Length(a));
}

vec3 v3RandomUnitSphere()
{
	/*int attempts = 0;
	do
	{
		float x = floatRandom() * 2 - 1;
		float y = floatRandom() * 2 - 1;
		float z = floatRandom() * 2 - 1;
		if ((x*x + y * y + z * z) < 1.0f)
			return (vec3) { x, y, z };
		attempts++;
	} while (attempts < 5);
	return v3Unit((vec3) { floatRandom(), floatRandom(), floatRandom() });*/

	/*Todo: Learn this math*/
	/*https://karthikkaranth.me/blog/generating-random-points-in-a-sphere/*/
	float u = floatRandom();
	float v = floatRandom();
	float theta = u * 2.0f * (float)PI;
	float phi = acosf(2.0f * v - 1.0f);
	float r = cbrtf(floatRandom());
	float sinTheta = sinf(theta);
	float cosTheta = cosf(theta);
	float sinPhi = sinf(phi);
	float cosPhi = cosf(phi);
	float x = r * sinPhi * cosTheta;
	float y = r * sinPhi * sinTheta;
	float z = r * cosPhi;
	return (vec3) { x, y, z };
}

vec3 v3Reflect(vec3 v, vec3 n)
{
	return v3Sub(v, v3Multf(v3Multf(n, v3Dot(v, n)), 2.0f));
}

int v3Refract(vec3 v, vec3 n, float ni_over_nt, vec3 * ref)
{
	vec3 uv = v3Unit(v);
	float dt = v3Dot(uv, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt*(1.0f - dt * dt);
	if (discriminant > 0)
	{
		*ref = v3Sub(v3Multf(v3Sub(uv, v3Multf(n, dt)), ni_over_nt), v3Multf(n, sqrtf(discriminant)));
		return 1;
	}
	return 0;
}
