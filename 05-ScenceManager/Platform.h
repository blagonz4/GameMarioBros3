#pragma once
#include "define.h"
#include "GameObject.h"

class Platform : public CGameObject
{
	float width, height;
public:
	Platform(float w, float h);
	virtual void Render(){}
	virtual void GetBoundingBox(float &l, float &t, float &r, float& b);
	float GetWidth() { return width; }
	//Platform();
	//~Platform();
};

