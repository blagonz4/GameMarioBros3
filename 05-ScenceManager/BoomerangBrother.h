#pragma once
#include "GameObject.h"
#include "define.h"
#include "Boomerang.h"
#include "PlayScence.h"
#include "Mario.h"
//--------------------------BOOMERANG BROTHER---------------------------
#define BOOMERANG_BROTHER_ANI_AIM_LEFT					0
#define BOOMERANG_BROTHER_ANI_THROW_LEFT				1
#define BOOMERANG_BROTHER_ANI_AIM_RIGHT					2
#define BOOMERANG_BROTHER_ANI_THROW_RIGHT				3

#define BOOMERANG_BROTHER_AIM_TIME					1000
#define BOOMERANG_BROTHER_RELOAD_TIME				2000
#define BOOMERANG_BROTHER_THROW_TIME				500
#define BOOMERANG_BROTHER_CHANGE_TIME				1000

#define BOOMERANG_BROTHER_LIMIT_RANGE				48
#define BOOMERANG_BROTHER_ACTIVE_RANGE				80
#define BOOMERANG_BROTHER_SPEED						0.025f
#define BOOMERANG_BROTHER_GRAVITY					0.001f
#define BOOMERANG_BROTHER_DEFLECT_SPEED				0.35f

#define BOOMERANG_BROTHER_STATE_FORWARD				1
#define BOOMERANG_BROTHER_STATE_DIE					2

#define BOOM_BROTHER_BBOX_WIDTH						16
#define BOOM_BROTHER_BBOX_HEIGHT					24
#define BOOMERANG_BROTHER_BOOMERANGS				2
class BoomerangBrother : public CGameObject
{
	DWORD aim_start = 0;
	DWORD throw_start = 0;
	DWORD reload_start = 0;
	DWORD change_start = 0;
	int bmrIndex = 0;
public:
	vector <CBoomerang*> ListBoomerang;
	CMario* mario;
	float start_x = 0;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void CreateBoomerang();
	float GetMarioRangeCurrent();
	BoomerangBrother(float x, float y,float direction);
	~BoomerangBrother();
	void SetState(int state);
	void StartAim() { aim_start = GetTickCount(); }
	void StartThrow() { throw_start = GetTickCount(); }
	void StartReload() { reload_start = GetTickCount(); }
	void StartChange() { change_start = GetTickCount(); }
};

