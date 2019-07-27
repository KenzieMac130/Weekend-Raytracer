#include "camera.h"
#include "platformUtil.h"
#include "extraMath.h"
#include <math.h>

#include <stdlib.h>

float antiAliasJitter(float amount)
{
	/*Todo: Figure out low discrepancy noise*/
	return ((floatRandom())*2.0f - 1.0f)*amount;
}

camera camera_init(vec3 pos, vec3 dir, float fov, int dimX, int dimY)
{
	camera cam;
	float aspect = (float)dimX / (float)dimY;
	float theta = fov * (float)PI / 180;
	float halfHeight = tanf(theta / 2);
	float halfWidth = aspect * halfHeight;
	cam.origin = pos;
	vec3 u, v, w;
	w = v3Unit(dir);
	u = v3Unit(v3Cross((vec3) { 0, 1, 0 }, w));
	v = v3Cross(w, u);
	//cam.lowerLeftCorner = v3Sub(pos, v3Sub(v3Multf(u, halfWidth), v3Sub(v3Multf(v, halfHeight), w)));
	//cam.horizontal = v3Multf(v3Multf(u, halfWidth), 2);
	//cam.horizontal = v3Multf(v3Multf(v, halfHeight), 2);
	cam.lowerLeftCorner = (vec3){ -halfWidth, -halfHeight, -1.0f };
	cam.horizontal = (vec3) { halfWidth*2 , 0.0f, 0.0f };
	cam.vertical = (vec3){ 0.0f, halfHeight*2, 0.0f };
	return cam;
}

ray camera_getRay(camera cam, float u, float v)
{
	vec3 dir = v3Add(cam.lowerLeftCorner, v3Add(v3Multf(cam.horizontal, u), v3Multf(cam.vertical, v)));
	return (ray){ cam.origin, dir };
}
