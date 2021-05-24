#pragma once
#include "Utils.h"
#include "GameObject.h"
#include "define.h"

class CMario : public CGameObject
{
	DWORD untouchable_start;
	DWORD limitjump_start;
	DWORD limitfly_start;
	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	bool isOnGround = true;
	bool isSitting;
	bool isHolding;
	int untouchable;
	bool isJumping = false;
	bool isFlying = false;
	int level;
	CMario(float x = 0.0f, float y = 0.0f);
	vector<LPGAMEOBJECT> listFire;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	int GetLevel(){return level;}
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartLimitJump() { isJumping = true; limitjump_start = GetTickCount(); }
	void StartLimitFly() { isFlying = true; limitfly_start = GetTickCount(); }
	void Reset();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};