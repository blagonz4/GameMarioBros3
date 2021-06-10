#pragma once
#include "GameObject.h"
#include "define.h"


class CGoomba : public CGameObject
{;
	float model;	
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	DWORD timeToDie;
public: 	
	int Health;
	CGoomba(float model, float direction);
	virtual void SetState(int state);
	float GetModel() { return model; }
};