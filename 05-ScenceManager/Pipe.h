#pragma once
#include "GameObject.h"
class Pipe : public CGameObject
{
public:
	int model;
	Pipe(int model);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~Pipe();
};
