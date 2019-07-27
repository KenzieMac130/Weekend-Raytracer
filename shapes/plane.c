#include "plane.h"

/*Todo: fix?*/
int plane_hit(void* self, ray r, float t_min, float t_max, hit_record* rec)
{
	plane pln = *(plane*)self;
	float denominator = v3Dot(pln.normal, r.dir);
	if (abs(denominator) > 0.0001f)
	{
		float t = v3Dot(v3Sub(pln.center, r.pos), pln.normal) / denominator;
		if (t < t_max && t > t_min)
		{
			rec->normal = pln.normal;
			rec->t = t;
			rec->p = rayPointAtParam(r, t);
			rec->pMat = pln.pMat;
			return 1;
		}
	}
	return 0;
}

plane plane_init(vec3 center, vec3 dir, float radius, void * pMat)
{
	plane pln;
	pln.fpHit = plane_hit;
	pln.center = center;
	pln.normal = dir;
	pln.radius = radius;
	pln.pMat = pMat;
	return pln;
}
