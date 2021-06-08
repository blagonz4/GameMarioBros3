#include "EffectPoint.h"

EffectPoint::EffectPoint(float X, float Y, float model)
{
	this->model = model;
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_POINT_EFFECT));
	eType = Type::EFFECT_POINT;
	minY = y - POINT_EFFECT_MIN_Y;
	vy = -POINT_EFFECT_SPEED_Y;
}
void EffectPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
	DebugOut(L"isFinish \n");
	if (y <= minY)
	{
		isFinish = true;
	}
}
void EffectPoint::Render()
{
	int ani = -1;
	switch ((int)model)
	{
	case POINT_EFFECT_MODEL_100:
		ani = POINT_EFFECT_ANI_100;
		break;
	case POINT_EFFECT_MODEL_200:
		ani = POINT_EFFECT_ANI_200;
		break;
	case POINT_EFFECT_MODEL_400:
		ani = POINT_EFFECT_ANI_400;
		break;
	case POINT_EFFECT_MODEL_800:
		ani = POINT_EFFECT_ANI_800;
		break;
	case POINT_EFFECT_MODEL_1K:
		ani = POINT_EFFECT_ANI_1K;
		break;
	case POINT_EFFECT_MODEL_2K:
		ani = POINT_EFFECT_ANI_2K;
		break;
	case POINT_EFFECT_MODEL_4K:
		ani = POINT_EFFECT_ANI_4K;
		break;
	case POINT_EFFECT_MODEL_8K:
		ani = POINT_EFFECT_ANI_8K;
		break;
	}
	animation_set->at(ani)->Render(x, y);
}

void EffectPoint::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = top = right = bottom = 0;
}
EffectPoint::~EffectPoint()
{
}
