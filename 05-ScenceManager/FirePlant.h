#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"
#include "PlayScence.h"
class FirePlant : public CGameObject
{
	CMario *mario;
	vector<LPGAMEOBJECT> listFire;

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

