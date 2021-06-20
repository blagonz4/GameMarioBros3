#pragma once
#include "GameObject.h"
#include "define.h"

class CBrick : public CGameObject
{
public:
	int isUnbox = 0;
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};