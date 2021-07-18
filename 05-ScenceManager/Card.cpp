#include "Card.h"

Card::Card( float cgLeft, float cgTop, float cgRight, float cgBottom)
{
	this->cgLeft = cgLeft;
	this->cgUp = cgTop;
	this->cgRight = cgRight;
	this->cgDown = cgBottom;
	eType = Type::CARD;
}

void Card::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}
void Card::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (eType != BUSH)
	{
		left = x + 2;
		top = y + 2;
		right = left + OBJECT_BBOX_WIDTH;
		bottom = top + OBJECT_BBOX_WIDTH;
	}
}
void Card::Render()
{
	animation_set->at(0)->Render(x, y);
}


Card::~Card()
{
}
