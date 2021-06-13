#include "Bush.h"



Bush::Bush()
{
}
void Bush::Render()
{
	animation_set->at(0)->Render(x, y);
}
void Bush::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

Bush::~Bush()
{
}
