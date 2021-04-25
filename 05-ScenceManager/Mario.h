#pragma once
#include "GameObject.h"
#include "define.h"


class CMario : public CGameObject
{
	int level;
	int untouchable;
	
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	bool isOnGround = true;
	bool isJumping = true;
	bool isSitting;
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	int GetLevel(){return level;}
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();
	void WalkRight();
	void WalkLeft();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};