#pragma once
#include "GameObject.h"
#include "define.h"
#include "FireBall.h"
#include "PlayScence.h"
#include "Mario.h"
//-----------------------------FIRE PLANT----------------------------
#define DISTANCE_SAFE_ZONE				25

#define LOAD_FIRE_PLANT_FROM_FILE		 6

#define LEFT_TOP_SIDE_NEAR				 1
#define LEFT_TOP_SIDE_FAR				 2
#define LEFT_BOTTOM_SIDE_NEAR			 3
#define LEFT_BOTTOM_SIDE_FAR			 4
#define RIGHT_TOP_SIDE_NEAR				 5
#define RIGHT_TOP_SIDE_FAR				 6
#define RIGHT_BOTTOM_SIDE_NEAR			 7
#define RIGHT_BOTTOM_SIDE_FAR			 8
#define LEFT_SIDE						 9
#define RIGHT_SIDE						10

#define FIRE_ENEMY_SPEED_X_NEAR			0.023f
#define FIRE_ENEMY_SPEED_Y_NEAR			0.025f

#define FIRE_ENEMY_SPEED_X_FAR			0.045f
#define FIRE_ENEMY_SPEED_Y_FAR			0.023f

#define FIRE_PLANT_BBOX_WIDTH			16
#define FIRE_PLANT_RED_BBOX_HEIGHT		32
#define FIRE_PLANT_GREEN_BBOX_HEIGHT	24
//----------------------Red Plant---------------------
#define	FIRE_PLANT_RED_ANI_LEFT_TOP					0
#define FIRE_PLANT_RED_ANI_LEFT_BOTTOM				1
#define FIRE_PLANT_RED_ANI_RIGHT_TOP				2
#define FIRE_PLANT_RED_ANI_RIGHT_BOTTOM				3
#define FIRE_PLANT_RED_ANI_ATTACK_LEFT_TOP			4
#define FIRE_PLANT_RED_ANI_ATTACK_LEFT_BOTTOM		5
#define FIRE_PLANT_RED_ANI_ATTACK_RIGHT_TOP			6
#define FIRE_PLANT_RED_ANI_ATTACK_RIGHT_BOTTOM		7
//----------------------Green Plant---------------------
#define	FIRE_PLANT_GREEN_ANI_LEFT_TOP				8
#define FIRE_PLANT_GREEN_ANI_LEFT_BOTTOM			9
#define FIRE_PLANT_GREEN_ANI_RIGHT_TOP				10
#define FIRE_PLANT_GREEN_ANI_RIGHT_BOTTOM			11
#define PLANT_SPEED_GROW_UP							0.02f
#define PLANT_SPEED_HIDDING				0.02f
#define PIRANHAPLANT_DELAY_TIME			750
#define PIRANHAPLANT_AIM_TIME			750
#define PIRANHAPLANT_DIYING_TIME		250
#define DISTANCE_FROM_MARIO_TO_PLANT	60

#define PIRANHAPLANT_STATE_SHOOTING		500

#define	FIRE_PLANT_RED					1
#define FIRE_PLANT_GREEN				2
//----------------------Bite Plant---------------------
#define BITE_PLANT								3
#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_BBOX_HEIGHT				24
#define PIRANHAPLANT_DARTING_SPEED				0.03f

#define PIRANHAPLANT_STATE_DARTING				100
#define PIRANHAPLANT_STATE_BITING				200
#define PIRANHAPLANT_STATE_DEATH				300
#define PIRANHAPLANT_STATE_INACTIVE				400

#define PIRANHAPLANT_ACTIVE_RANGE				20

#define PIRANHAPLANT_ANI_DARTING				12
#define PIRANHAPLANT_ANI_DEATH					1

#define PIRANHAPLANT_BITING_TIME				1000
#define PIRANHAPLANT_DIYING_TIME				250

class FirePlant : public CGameObject
{
	//FIRE PLANT
	CMario *mario;

	DWORD shooting_start = 0;
	DWORD aim_start = 0;
	DWORD delay_start = 0;

	bool isHiding;
	bool isGrowUp;
	bool isAttacking;
	bool isSafe;
	//PIRANHAPLANT
	DWORD biting_start = 0;
	DWORD dying_start = 0;
	float limitY = 0;
	float plantHeight = 0;
public:
	vector<FireBall*> listFire;
	float model;
	int marioRange;
	float vxFire, vyFire;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void ShootFire(int marioRange);
	int GetCurrentMarioRange();
	bool GetSafeZone();
	FirePlant(float x, float y, CMario* mario, float model);

	void StartShooting() { shooting_start = GetTickCount(); }
	void StartAim() { aim_start = GetTickCount(); }
	void StartDelay() { delay_start = GetTickCount(); }
	void StartDying() { dying_start = GetTickCount(); }
	void SetLimitY(float ly) { limitY = ly - PIRANHAPLANT_BBOX_HEIGHT; }
	void StartBitting() { biting_start = GetTickCount(); }

	~FirePlant();
};

