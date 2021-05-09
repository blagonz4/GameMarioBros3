#pragma once
#include "GameObject.h"

class ColorBlock : public CGameObject
{

public:
	ColorBlock(float w, float h);
	virtual void Render() {}
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
