#pragma once
#include "GameObject.h"
#include "define.h"
class EffectBrokenBrick : public CGameObject
{
public:
	float deflectY;
	EffectBrokenBrick(float X, float Y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	~EffectBrokenBrick();
};

