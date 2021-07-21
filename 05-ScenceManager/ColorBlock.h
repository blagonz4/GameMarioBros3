#pragma once
#include "GameObject.h"
#include "Mario.h"
class ColorBlock : public CGameObject
{
	CMario *player;
public:
	float width, height = 0;
	ColorBlock(float w, float h,CMario* mario);
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
