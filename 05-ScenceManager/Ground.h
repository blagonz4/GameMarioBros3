#pragma once
#include "GameObject.h"
#include "define.h"
class Ground : public CGameObject
{
public:
	Ground(float X, float Y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	~Ground();
};

