#pragma once
#include "GameObject.h"
#include "define.h"

//------------------------BOOMERANG-------------------------
#define BOOMERANG_ANI								0
#define BOOMERANG_BBOX_WIDTH						15
#define BOOMERANG_BBOX_HEIGHT						16

#define LOAD_BOOMERANG_FROM_TXT						25

#define BOOMERANG_STATE_IDLE						0
#define BOOMERANG_STATE_1							1
#define BOOMERANG_STATE_2							2
#define BOOMERANG_STATE_3							3
#define BOOMERANG_STATE_4							4
#define BOOMERANG_STATE_5							5
#define BOOMERANG_STATE_6							6

#define BOOMERANG_SPEED_X							0.1f
#define BOOMERANG_SPEED_Y      						0.05f
#define BOOMERANG_GRAVITY		      				0.001f
#define BOOMERANG_ACCELERATION	      				0.001f
#define BOOMERANG_CHANGE_STATE_TIME					750

#define BOOMERANG_DIFF								3
#define BOOMERANG_SPEED_DIFF						1.25f
class CBoomerang : public CGameObject
{
	DWORD state_start = 0;
	//LPGAMEOBJECT boomerangbrother;
public:
	bool isAppear = false;
	float startX, startY;
	void StartChangState() { state_start = 1; }
	virtual void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	CBoomerang(float X, float Y, float dir);
	~CBoomerang();
};

