#pragma once

#include "../material.h"

typedef struct
{
	MATERIAL_BASE
	vec3 color;
	float roughness;
	float ior;
} mat_glass;

mat_glass mat_glass_init(vec3 color, float roughness, float ior);