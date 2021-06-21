#pragma once
#include "GameObject.h"
#include "define.h"
class Arrow : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	Arrow(float X, float Y);
	~Arrow();
};

