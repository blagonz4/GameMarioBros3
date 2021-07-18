#pragma once
#include "GameObject.h"
#include "define.h"
class Scene	: public CGameObject
{
public:
	float model;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	Scene(float model,float cgLeft, float cgTop, float cgRight, float cgBottom);
	~Scene();
};

