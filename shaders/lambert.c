#include "lambert.h"

int lambertScatter(void* self, ray r_in, hit_record* rec, rayOutput* output)
{
	mat_lambert mat = *(mat_lambert*)self;
	vec3 target = v3Add(v3Add(rec->p, rec->normal), v3RandomUnitSphere());
	output->scattered = (ray) { rec->p, v3Sub(target, rec->p) };
	output->attenuation = mat.color;
	output->albedo = mat.color;
	return 1;
}

mat_lambert mat_lambert_init(vec3 color)
{
	mat_lambert mat;
	mat.fpScatter = lambertScatter;
	mat.color = color;
	return mat;
}
