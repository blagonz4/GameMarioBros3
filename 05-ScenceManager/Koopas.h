#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"


class CKoopas : public CGameObject
{
	CMario* player;
	DWORD defend_start;
	int isDefend;
	void StartDefendTime() { isDefend = 1; defend_start = GetTickCount(); }
	virtual void GetBoundingBox(float &left, float &top,float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	bool isBeingHeld;
	CKoopas(float &model, float &direction,CMario* mario);
	virtual void SetState(int state);
};

