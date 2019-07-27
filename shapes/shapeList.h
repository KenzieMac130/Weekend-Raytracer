#pragma once

#include "../shape.h"

typedef struct
{
	SHAPE_BASE
	shapeBase** shapes;
	int shapeCount;
} shapeList;

shapeList shapeList_init(void** shapes, int count);