#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"
//-----------poop-------------
#define POOP_ANI					0
#define LOAD_POOP_FROM_TXT			26
#define POOP_BBOX					8
#define POOP_MAX_RIGHT_X			8
#define TIME_DISAPPEAR				5000
class Poop : public CGameObject
{
	DWORD timeDisappear;
public:
	float limitRight;
	float limitLeft;
	bool isAttach = false;//dinh mario
	CMario* mario;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void AttachToMario();
	Poop(float X, float Y, CMario* mario);
	~Poop();
};

