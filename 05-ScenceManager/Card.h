#pragma once
#include "GameObject.h"
#include "define.h"
class Card : public CGameObject
{
public:
	Card();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~Card();
};
