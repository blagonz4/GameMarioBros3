#pragma once
#include "GameObject.h"
#include "define.h"
#include "Koopas.h"
#include "Goomba.h"
class FireBall : public CGameObject
{
	
public:
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetPosition(float &x, float &y) { this->x = x; this->y = y; }
	FireBall(float X, float Y);
};

