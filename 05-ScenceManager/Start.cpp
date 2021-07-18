#include "Start.h"



Start::Start(float cgLeft, float cgTop, float cgRight, float cgBottom)
{
	this->cgLeft = cgLeft;
	this->cgUp = cgTop;
	this->cgRight = cgRight;
	this->cgDown = cgBottom;
	eType = Type::START;
}
void Start::Render()
{
	animation_set->at(0)->Render(x, y);
}
void Start::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}
void Start::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (eType != BUSH)
	{
		left = x + 2;
		top = y + 2;
		right = left + OBJECT_BBOX_WIDTH;
		bottom = top + OBJECT_BBOX_WIDTH;
	}
}

Start::~Start()
{
}
