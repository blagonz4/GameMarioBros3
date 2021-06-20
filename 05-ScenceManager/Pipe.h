#pragma once
#include "GameObject.h"
class Pipe : public CGameObject
{
public:
	float model;
	Pipe(float X, float Y,float model);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~Pipe();
};
