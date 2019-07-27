#pragma once

#include "../material.h"

typedef struct
{
	MATERIAL_BASE
	vec3 color;
	float roughness;
} mat_metalSimple;

mat_metalSimple mat_metalSimple_init(vec3 color, float roughness);