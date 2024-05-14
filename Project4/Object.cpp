#include "Object.h"

Object::Object(unsigned int r, unsigned int c, States st, bool onBarn)
	:onBarn(onBarn)
	, st(st)
{
	pos.w = width;
	pos.h = height;
	pos.x = c * height;
	pos.y = r * width;
}
