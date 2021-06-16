#include "Poop.h"



Poop::Poop(float X, float Y)
{
	this->x = X; this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_POOP_FROM_TXT));
	eType = Type::POOP;
	limitLeft = X;
	limitRight = X + LEAF_MAX_RIGHT_X;
}

void Poop::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + POOP_BBOX;
	bottom = y + POOP_BBOX;
}
void Poop::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (!CheckObjectInCamera(this))
	{
		isFinish = true;
	}
	x += dx;
	y += dy;

	if (x <= limitLeft)
	{
		vy = LEAF_GRAVITY * dt;
		vx = LEAF_SPEED_X * dt;
	}
	if (x >= limitRight)
	{
		vy = LEAF_GRAVITY * dt;
		vx = -LEAF_SPEED_X * dt;
	}
	else vy = LEAF_GRAVITY/2 * dt;
}
void Poop::Render()
{
	animation_set->at(POOP_ANI)->Render(x, y);
}

Poop::~Poop()
{
}
