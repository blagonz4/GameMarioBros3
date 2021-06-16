#pragma once
#include "GameObject.h"
#include "define.h"
#include "Poop.h"
#include "Mario.h"
class CGoomba : public CGameObject
{;
	DWORD timeDropDelay;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	DWORD timeToDie;
public: 	
	CMario* mario;
	float model;
	vector<LPGAMEOBJECT> listPoop;
	int Health;
	CGoomba(CMario* mario,float model, float direction);
	virtual void SetState(int state);
	void TurnAround();
	float GetModel() { return model; }
};