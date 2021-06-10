#pragma once
#include "GameObject.h"
#include "define.h"
class EffectTailHit : public CGameObject
{
	DWORD timeEffect;
public:
	void Update(DWORD dt, vector <LPGAMEOBJECT>*coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	EffectTailHit(float X, float Y);
	~EffectTailHit();
};

