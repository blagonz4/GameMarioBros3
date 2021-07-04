#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"
#include "PlayScence.h"
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

#define FIRE_ENEMY_SPEED_Y_FAR			0.000038f
#define FIRE_ENEMY_SPEED_Y_NEAR			0.000037f
#define FIRE_ENEMY_SPEED_X_NEAR			0.00009f
#define FIRE_ENEMY_SPEED_X_FAR			0.000085f

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

//----------------------Bite Plant---------------------
#define	BITE_PLANT_ANI_ATTACK						12
#define PLANT_SPEED_GROW_UP				0.02f
#define PLANT_SPEED_HIDDING				0.02f
#define TIME_DELAY_GROW_UP				1500
#define TIME_ATTACK_DELAY				1000
#define TIME_ATTACK						1500
#define DISTANCE_FROM_MARIO_TO_PLANT	60

#define FIRE_PLANT_STATE_GROW_UP		1
#define FIRE_PLANT_STATE_ATTACK			2
#define FIRE_PLANT_STATE_HIDING			3

#define	FIRE_PLANT_RED					1
#define FIRE_PLANT_GREEN				2
#define BITE_PLANT						3
class FirePlant : public CGameObject
{
	CMario *mario;

	DWORD timeDelayAttack;
	DWORD timeHidding;
	DWORD timeAttack;
	DWORD timeToAttack;
	DWORD timeToHide;

	bool isHiding;
	bool isGrowUp;
	bool isAttacking;
	bool isSafe;

public:
	vector<LPGAMEOBJECT> listFire;
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
	FirePlant(CMario* mario, float model);

	void StartGrowUp() { isGrowUp = true; timeToAttack = GetTickCount(); }
	void StartHiding() { isHiding = true; timeDelayAttack = GetTickCount(); }

	~FirePlant();
};

