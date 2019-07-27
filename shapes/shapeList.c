#include "shapeList.h"

int shapeList_hit(void* self, ray r, float t_min, float t_max, hit_record* rec)
{
	shapeList sList = *(shapeList*)self;
	hit_record temp_rec;
	int hitSomething = 0;
	float closest = t_max;
	for (int i = 0; i < sList.shapeCount; i++)
	{
		if (sList.shapes[i]->fpHit(sList.shapes[i], r, t_min, closest, &temp_rec))
		{
			hitSomething = 1;
			closest = temp_rec.t;
			*rec = temp_rec;
		}
	}
	return hitSomething;
}

shapeList shapeList_init(void ** shapes, int count)
{
	shapeList sList;
	sList.fpHit = shapeList_hit;
	sList.shapes = (shapeBase**)shapes;
	sList.shapeCount = count;
	return sList;
}