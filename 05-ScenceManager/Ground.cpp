#include "Ground.h"


Ground::Ground(float X, float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::GROUND;
}
void Ground::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}
void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void Ground::Render()
{
	animation_set->at(0)->Render(x, y);
}
