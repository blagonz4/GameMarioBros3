#pragma once
#include "GameObject.h"
#include "define.h"

class GoldBrick : public CGameObject
{
	DWORD timeTransform;
public:
	float model;
	int isUnbox;
	float startX, startY, minY;
	GoldBrick(float x, float y, float model);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	~GoldBrick();
};

