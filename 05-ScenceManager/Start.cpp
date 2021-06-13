#include "Start.h"



Start::Start()
{
}
void Start::Render()
{
	animation_set->at(0)->Render(x, y);
}
void Start::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}



Start::~Start()
{
}
