#pragma once
#include "GameObject.h"
#include "define.h"

class EffectCoin : public CGameObject
{
protected:
	float minY;
	float maxY;
	
public:
	bool isFalling = false;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	EffectCoin(float X, float Y);
	~EffectCoin();
};

