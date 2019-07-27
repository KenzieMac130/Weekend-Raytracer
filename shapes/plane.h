#include "../shape.h"
#include "../material.h"

typedef struct
{
	SHAPE_BASE
	vec3 center;
	vec3 normal;
	float radius;
	struct materialBase* pMat;
} plane;

plane plane_init(vec3 center, vec3 dir, float radius, void* pMat);