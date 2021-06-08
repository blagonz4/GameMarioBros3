#include "EffectBrokenBrick.h"



EffectBrokenBrick::EffectBrokenBrick(float X, float Y)
{
	//this->deflectY = defY;
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_EFFECT_BROKEN_BRICK));
	vx = EFFECT_BROKEN_BRICK_SPEED_X *dt;
	vy = -EFFECT_BROKEN_BRICK_SPEED_Y *dt;
	eType = Type::EFFECT_BROKEN_BRICK;
}

void EffectBrokenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += EFFECT_BROKEN_BRICK_GRAVITY * dt;
	x += dx;
	y += dy;
	if (!CheckObjectInCamera(this))
	{
		isFinish = true;
	}
}
void EffectBrokenBrick::Render()
{
	if (isFinish)
		return;
	int ani = EFFECT_BROKEN_BRICK_ANI;
	animation_set->at(ani)->Render(x, y);
}

void EffectBrokenBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0;
}
EffectBrokenBrick::~EffectBrokenBrick()
{
}
