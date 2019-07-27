#pragma once

#include "ray.h"

typedef struct
{
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
} camera;

float antiAliasJitter(float amount);

camera camera_init(vec3 pos, vec3 dir, float fov, int dimX, int dimY);

ray camera_getRay(camera cam, float u, float v);