#pragma once

#include "GameObject.h"
#include "define.h"
#include "Mario.h"
class CKoopas : public CGameObject
{
	DWORD defend_start;
	int isDefend;
	void StartDefendTime() { isDefend = 1; defend_start = GetTickCount(); }
	virtual void GetBoundingBox(float &left, float &top,float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CKoopas(float &model, int &direction);
	virtual void SetState(int state);
};