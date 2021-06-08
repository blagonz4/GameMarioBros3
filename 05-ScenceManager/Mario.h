#pragma once
#include "Utils.h"
#include "GameObject.h"
#include "define.h"


class CMario : public CGameObject
{

	DWORD limitjump_start;
	DWORD limitfly_start;
	DWORD timeTransform; 
	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	bool isOnGround = true;
	bool isSitting;
	bool isHolding;
	bool isTransformToBig = false;
	bool isTransformToRacoon = false;
	bool isJumping = false;
	bool isFlying = false;

	CMario(float x = 0.0f, float y = 0.0f);
	vector<LPGAMEOBJECT> listFire;
	vector<LPGAMEOBJECT> listEffect;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	int GetLevel(){return level;}
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartLimitJump() { isJumping = true; limitjump_start = GetTickCount(); }
	void StartLimitFly() { isFlying = true; limitfly_start = GetTickCount(); }
	void Reset();
	void ShowEffectPoint(CGameObject* obj, float model);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};