#include "glass.h"
#include "../extraMath.h"

/*Todo: fix glass*/
int glassScatter(void* self, ray r_in, hit_record* rec, rayOutput* output)
{
	mat_glass mat = *(mat_glass*)self;
	vec3 outward_normal;
	vec3 reflected = v3Reflect(r_in.dir, rec->normal);
	float ni_over_nt;
	vec3 refracted;
	float reflectProbability;
	float cosine;
	if (v3Dot(r_in.dir, rec->normal) > 0)
	{
		outward_normal = v3Neg(rec->normal);
		ni_over_nt = mat.ior;
		cosine = mat.ior * v3Dot(r_in.dir, rec->normal) / v3Length(r_in.dir);
	}
	else
	{
		outward_normal = rec->normal;
		ni_over_nt = 1.0f / mat.ior;
		cosine = -v3Dot(r_in.dir, rec->normal) / v3Length(r_in.dir);
	}
	if (v3Refract(r_in.dir, outward_normal, ni_over_nt, &refracted))
	{
		reflectProbability = 1.0f-fresnel_schlick(cosine, mat.ior);
	}
	else
	{
		reflectProbability = 1.0f;
	}
	if (floatRandom() < reflectProbability)
	{
		output->scattered = (ray) { rec->p, reflected };
	}
	else
	{
		output->scattered = (ray) { rec->p, refracted };
	}
	output->attenuation = v3Lerp(mat.color, (vec3) { 1, 1, 1 }, fresnel_schlick(cosine, mat.ior));
	return 1;
}

mat_glass mat_glass_init(vec3 color, float roughness, float ior)
{
	mat_glass mat;
	mat.fpScatter = glassScatter;
	mat.color = color;
	mat.roughness = roughness;
	mat.ior = ior;
	return mat;
}
