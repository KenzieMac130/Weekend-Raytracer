#pragma once

#include "ray.h"

typedef struct
{
	float t;
	vec3 p;
	vec3 normal;
	struct materialBase* pMat;
} hit_record;

typedef int (*hitFunc_t)(void* self, ray r, float t_min, float t_max, hit_record* rec);

#define SHAPE_BASE hitFunc_t fpHit;

/*Material*/

typedef struct
{
	ray scattered;
	vec3 attenuation;
	vec3 emission;
	vec3 albedo;
} rayOutput;

typedef int(*scatterFunc_t)(void* self, ray r_in, hit_record* rec, rayOutput *output);

typedef struct
{
	SHAPE_BASE
} shapeBase;

#define MATERIAL_BASE \
scatterFunc_t fpScatter;

struct materialBase
{
	MATERIAL_BASE
};