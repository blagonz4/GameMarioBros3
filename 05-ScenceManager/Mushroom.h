#pragma once
#include "GameObject.h"
#include "define.h"

class Mushroom : public CGameObject
{
public:
	float startY;
	bool isMoving;
	float minY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();	
	void SetState(int state);
	float GetModel() { return model; }
	Mushroom(float X, float Y,float model);
	~Mushroom();
};

