#pragma once
#include "GameObject.h"
#include "define.h"
#include "Poop.h"
#include "Mario.h"
#include "PlayScence.h"
//-----------------------------Goomba---------------------------
#define GOOMBA_WALKING_SPEED					0.05f

#define GOOMBA_BBOX_WIDTH						16
#define GOOMBA_WING_BBOX_WIDTH					20
#define GOOMBA_BBOX_HEIGHT						15
#define GOOMBA_WING_BBOX_HEIGHT					24
#define GOOMBA_BBOX_HEIGHT_DIE					9

#define GOOMBA_MODEL_NORMAL						1
#define GOOMBA_MODEL_WING						2
#define GOOMBA_MODEL_WING_BROWN					3

#define GOOMBA_STATE_DIE						100

#define GOOMBA_ANI_DIE							0
#define GOOMBA_ANI_WING_DIE						1
#define GOOMBA_ANI_WALKING						2
#define GOOMBA_ANI_WING_WALKING_WITH_OUT_WING	3
#define GOOMBA_ANI_WING_WALKING					4
#define GOOMBA_ANI_WING_JUMPING					5
#define GOOMBA_ANI_WING_FALLING					6
#define GOOMBA_ANI_WING_BROWN_JUMPING			7
#define GOOMBA_ANI_WING_BROWN_FALLING			8

#define GOOMBA_JUMPING_SPEED					0.25f
#define TIME_TO_DIE								500
#define POOP_DELAY_DROP							1000
class CGoomba : public CGameObject
{
	DWORD timeDropDelay;
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
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};