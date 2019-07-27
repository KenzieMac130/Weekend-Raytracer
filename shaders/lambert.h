#pragma once

#include "../material.h"

typedef struct
{
	MATERIAL_BASE
	vec3 color;
} mat_lambert;

mat_lambert mat_lambert_init(vec3 color);