#pragma once
#include "GameObject.h"
#include "define.h"
class EffectDisappear : public CGameObject
{
	DWORD timeEffect;
public:
	void Update(DWORD dt, vector <LPGAMEOBJECT>*coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	EffectDisappear(float X, float Y);
	~EffectDisappear();
};

