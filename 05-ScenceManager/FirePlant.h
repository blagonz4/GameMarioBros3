#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"
#include "PlayScence.h"
class FirePlant : public CGameObject
{
	vector<FirePlant*> *listFirePlant;

	float maxDistanceY;
	float minDistanceY;
	int marioRange;
	CMario *mario;
	DWORD timeAttackDelay;
	DWORD timeHidding;
	DWORD timeAttack;
	bool isHidding;
	bool isGrowUp;
	bool isSafe;
public:
	bool isFinish = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void SetFireFly(int marioRange);
	void ShootFire();
	int GetCurrentMarioRange();
	bool GetSafeZone();
	void CreateFirePlant();
	FirePlant(float X, float Y, vector<FirePlant*> *listFirePlant);
	~FirePlant();
};

