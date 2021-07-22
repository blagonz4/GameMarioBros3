#pragma once
#include "GameObject.h"
#include "define.h"

class Coin : public CGameObject
{

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	Coin();
	~Coin();
};
