#include "Bush.h"



Bush::Bush()
{
	eType = Type::BUSH;
}
void Bush::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}
void Bush::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (eType != BUSH)
	{
		left = x + 2;
		top = y + 2;
		right = left + OBJECT_BBOX_WIDTH;
		bottom = top + OBJECT_BBOX_WIDTH;
	}
}
void Bush::Render()
{
	animation_set->at(0)->Render(x, y);
}

Bush::~Bush()
{
}
