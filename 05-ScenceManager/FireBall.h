#pragma once
#include "GameObject.h"
#include "define.h"
#include "Koopas.h"
#include "Goomba.h"
#include "Pipe.h"
#include "BoomerangBrother.h"

//----------------------FIRE BALL-------------------
#define FIRE_SPEED						0.008f
#define FIRE_GRAVITY					0.0006f
#define FIRE_BOUNCE_SPEED_Y				0.15f
#define FIRE_BBOX_WIDTH					8
#define FIRE_BBOX_HEIGHT				8
#define SHOOT_FIRE_RIGHT				0
#define SHOOT_FIRE_LEFT					1
#define LOAD_FIRE_FROM_FILE				5
class FireBall : public CGameObject
{
	float defineVx, defineVy;
	
public:
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetPosition(float &x, float &y) { this->x = x; this->y = y; }
	FireBall(float X, float Y,float defineVx, float defineVy);
};

