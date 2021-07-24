#pragma once
#include "define.h"
#include "GameObject.h"
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
class Platform : public CGameObject
{
	float width, height;
public:
	Platform();
	virtual void Render() { } //RenderBoundingBox(); }
	virtual void GetBoundingBox(float &l, float &t, float &r, float& b);
	float GetWidth() { return width; }
	//Platform();
	//~Platform();
};

