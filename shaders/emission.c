#include "emission.h"

/*todo: complete*/
int emitScatter(void* self, ray r_in, hit_record* rec, rayOutput* output)
{
	mat_emit mat = *(mat_emit*)self;
	output->scattered = (ray) { rec->p, v3Neg(rec->normal) };
	output->attenuation = (vec3) { 0, 0, 0 };
	output->emission = mat.color;
	return 0;
}

mat_emit mat_emit_init(vec3 color)
{
	mat_emit mat;
	mat.fpScatter = emitScatter;
	mat.color = color;
	return mat;
}
