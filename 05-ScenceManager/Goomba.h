#pragma once
#include "GameObject.h"
#include "define.h"


class CGoomba : public CGameObject
{
	int model;	
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public: 	
	int Health;
	CGoomba(int model, int direction);
	virtual void SetState(int state);
};