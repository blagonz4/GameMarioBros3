#pragma once
#include "GameObject.h"
#include "define.h"
class Start : public CGameObject
{
public:
	Start(float cgLeft,float cgTop,float cgRight,float cgBottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	~Start();
};
