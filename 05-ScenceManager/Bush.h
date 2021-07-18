#pragma once
#include "define.h"
#include "GameObject.h"
class Bush : public CGameObject
{
public:
	Bush();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	~Bush();
};

