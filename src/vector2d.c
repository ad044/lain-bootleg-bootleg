#include "vector2d.h"

#include <math.h>

float dist_between(Vector2D v1, Vector2D v2)
{
	return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
}
