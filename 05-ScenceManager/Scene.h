#pragma once
#include "GameObject.h"
#include "define.h"
class Scene	: public CGameObject
{
public:
	float model;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects);
	Scene(float model,float cgLeft, float cgTop, float cgRight, float cgBottom);
	~Scene();
};

