#include "sphere.h"

/*Todo: Figure out what math does*/
int sphere_hit(void* self, ray r, float t_min, float t_max, hit_record* rec)
{
	sphere spr = *(sphere*)self;
	vec3 oc = v3Sub(r.pos, spr.center); /*Offset position*/
	float a = v3Dot(r.dir, r.dir); /*???*/
	float b = v3Dot(oc, r.dir); /*???*/
	float c = v3Dot(oc, oc) - spr.radius * spr.radius; /*???*/
	float disciminant = b*b - a*c; /*???*/
	if (disciminant > 0) /*Inside sphere*/
	{
		float temp = (-b - sqrtf(b*b - a * c)) / a; /*Uhhhh...*/
		if (temp < t_max && temp > t_min)
		{
			rec->t = temp;
			rec->p = rayPointAtParam(r, temp);
			rec->normal = v3Divf(v3Sub(rec->p, spr.center), spr.radius);
			rec->pMat = spr.pMat;
			return 1;
		}
		temp = (-b + sqrtf(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec->t = temp;
			rec->p = rayPointAtParam(r, temp);
			rec->normal = v3Divf(v3Sub(rec->p, spr.center), spr.radius);
			rec->pMat = spr.pMat;
			return 1;
		}
	}
	return 0;
}

sphere sphere_init(vec3 center, float radius, void* pMat)
{
	sphere spr;
	spr.fpHit = sphere_hit;
	spr.center = center;
	spr.radius = radius;
	spr.pMat = pMat;
	return spr;
}
