#include "Platform.h"



Platform::Platform()
{
	eType = Type::PLATFORM;
}

void Platform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
