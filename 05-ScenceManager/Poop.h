#pragma once
#include "GameObject.h"
#include "define.h"
class Poop : public CGameObject
{
public:
	float limitRight;
	float limitLeft;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	Poop(float X, float Y);
	~Poop();
};

