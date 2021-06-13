#pragma once
#include "GameObject.h"
#include "define.h"
class Start : public CGameObject
{
public:
	Start();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~Start();
};
