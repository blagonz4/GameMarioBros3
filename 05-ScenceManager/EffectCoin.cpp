#include "EffectCoin.h"



EffectCoin::EffectCoin(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_EFFECT_COIN));
	eType = Type::EFFECT_COIN;
	minY = Y - COIN_EFFECT_MIN_Y;
	maxY = Y - COIN_EFFECT_MAX_Y;
	vy = -COIN_EFFECT_SPEED_Y ;
}

void EffectCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	y += dy;
	if (y <= minY)
	{
		vy = COIN_EFFECT_SPEED_Y;
		isFalling = true;
	}
	if (isFalling)
	{
		if (y >= maxY)
		{
			isFinish = true;
		}
	}

}

void EffectCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0;
}
void EffectCoin::Render()
{
	int ani = COIN_EFFECT_ANI;
	animation_set->at(ani)->Render(x, y);
}
EffectCoin::~EffectCoin()
{
}
