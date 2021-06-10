#include "EffectDisappear.h"

EffectDisappear::EffectDisappear(float X, float Y)
{
	this->x = X+5;
	this->y = Y+7;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_EFFECT_DISAPPEAR));
	eType = Type::EFFECT_DISAPPEAR;
}
void EffectDisappear::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	timeEffect += dt;
	if (timeEffect>=TIME_EFFECT_DISAPEAR)
	{
		isFinish = true;
	}
}
void EffectDisappear::Render()
{
	int ani = DISAPPEAR_EFFECT_ANI;
	animation_set->at(ani)->Render(x, y);
}

void EffectDisappear::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = top = right = bottom = 0;
}

EffectDisappear::~EffectDisappear()
{
}
