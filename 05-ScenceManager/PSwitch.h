#pragma once
#include "GameObject.h"
#include "define.h"

class PSwitch : public CGameObject
{
public:
	bool isUsed = false;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject);
	PSwitch(float X, float Y);
	~PSwitch();
};

