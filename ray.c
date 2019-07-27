#include "ray.h"

vec3 rayPointAtParam(ray r, float t)
{
	return v3Add(r.pos, v3Multf(r.dir, t));
}
