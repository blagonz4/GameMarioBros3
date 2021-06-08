#pragma once
#include "GameObject.h"
#include "define.h"

class EffectPoint : public CGameObject
{
public:
	float model;
	float minY;
	EffectPoint(float X, float Y, float model);
	void Update(DWORD dt, vector <LPGAMEOBJECT>*coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	float GetModel() { return model; }
	~EffectPoint();
};

