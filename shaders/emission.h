#pragma once

#include "../material.h"

typedef struct
{
	MATERIAL_BASE
	vec3 color;
} mat_emit;

mat_emit mat_emit_init(vec3 color);