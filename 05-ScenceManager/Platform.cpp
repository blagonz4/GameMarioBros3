#include "Platform.h"



Platform::Platform(float w, float h)
{
	//eType = Type::PLATFORM;
	this->width = w;
	this->height = h;
}


void Platform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
