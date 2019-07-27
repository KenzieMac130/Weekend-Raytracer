#pragma once

#include "vec3.h"

typedef struct
{
	vec3 pos;
	vec3 dir;
} ray;

vec3 rayPointAtParam(ray r, float t);