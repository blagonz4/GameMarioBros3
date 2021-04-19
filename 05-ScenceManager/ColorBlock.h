#pragma once
#include "GameObject.h"
class ColorBlock : public CGameObject
{

public:
	ColorBlock(float w, float h);
	virtual void Render() {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
