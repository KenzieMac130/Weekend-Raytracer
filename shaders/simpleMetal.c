#include "simpleMetal.h"

int metalScatter(void* self, ray r_in, hit_record* rec, rayOutput* output)
{
	mat_metalSimple mat = *(mat_metalSimple*)self;
	vec3 reflected = v3Reflect(v3Unit(r_in.dir), rec->normal);
	output->scattered = (ray) { rec->p, v3Add(reflected, v3Multf(v3RandomUnitSphere(), mat.roughness)) };
	output->attenuation = mat.color;
	return (v3Dot(output->scattered.dir, rec->normal)>0);
}

mat_metalSimple mat_metalSimple_init(vec3 color, float roughness)
{
	mat_metalSimple mat;
	mat.fpScatter = metalScatter;
	mat.color = color;
	mat.roughness = roughness;
	return mat;
}
