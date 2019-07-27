#pragma once

#include "../shape.h"
#include "../material.h"

typedef struct
{
	SHAPE_BASE
	vec3 center;
	float radius;
	struct materialBase* pMat;
} sphere;

sphere sphere_init(vec3 center, float radius, void* pMat);