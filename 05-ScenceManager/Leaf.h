#pragma once
#include "GameObject.h"
#include "define.h"

class Leaf : public CGameObject
{
public:
	bool isFalling;
	float minY;
	float limitRight;
	float limitLeft;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	Leaf(float X, float Y);
	void SetState(int state);
	~Leaf();
};

