#pragma once
#include "GameObject.h"
#include "define.h"
class Boomerang : public CGameObject
{
	bool isTurning = false;
	bool isComingBack = false;
public:
	float startX, startY;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	Boomerang(float X, float Y, int dir);
	~Boomerang();
};

