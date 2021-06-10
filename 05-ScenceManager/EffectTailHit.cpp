#include "EffectTailHit.h"



EffectTailHit::EffectTailHit(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_EFFECT_TAIL_HIT));
	eType = Type::EFFECT_TAIL_HIT;
}

void EffectTailHit::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	timeEffect += dt;
	if (timeEffect >= TIME_EFFECT_DISAPEAR)
	{
		isFinish = true;
	}
}
void EffectTailHit::Render()
{
	int ani = TAIL_HIT_EFFECT_ANI;
	animation_set->at(ani)->Render(x, y);
}

void EffectTailHit::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = top = right = bottom = 0;
}


EffectTailHit::~EffectTailHit()
{
}
