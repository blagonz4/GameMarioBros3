#pragma once
#include "GameObject.h"
#include "define.h"
#include "BoomerangBrother.h"
//------------------------BOOMERANG-------------------------
#define BOOMERANG_ANI								0
#define BOOMERANG_BBOX_WIDTH						15
#define BOOMERANG_BBOX_HEIGHT						16
#define LOAD_BOOMERANG_FROM_TXT						25
#define BOOMERANG_FLY_MIN_HEIGHT					40
#define BOOMERANG_SPEED_X							0.002f
#define BOOMERANG_SPEED_Y							0.001f
class Boomerang : public CGameObject
{
	bool isTurning = false;
	bool isComingBack = false;
	LPGAMEOBJECT boomerangbrother;
public:
	float startX, startY;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	Boomerang(float X, float Y, float dir, LPGAMEOBJECT bb);
	~Boomerang();
};

