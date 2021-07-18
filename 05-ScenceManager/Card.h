#pragma once
#include "GameObject.h"
#include "define.h"
class Card : public CGameObject
{
public:
	Card( float cgLeft, float cgTop, float cgRight, float cgBottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void Render();
	~Card();
};
