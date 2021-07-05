#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"
#include "Boomerang.h"
#include "PlayScence.h"

//--------------------------BOOMERANG BROTHER---------------------------
#define BOOM_BROTHER_STATE_WALKING					100
#define BOOM_BROTHER_STATE_ATTACK					200
#define	BOOM_BROTHER_STATE_DIE						300
#define BOOM_BROTHER_ANI_RIGHT						0
#define BOOM_BROTHER_ANI_LEFT						1
#define BOOM_BROTHER_SPEED_X						0.00015f
#define BOOM_BROTHER_BBOX_WIDTH						16
#define BOOM_BROTHER_BBOX_HEIGHT					24
#define TIME_BB_WALKING								2000
#define TIME_BB_ATTACK								1500
class BoomerangBrother : public CGameObject
{
public:
	vector<LPGAMEOBJECT> ListBoomerang;
	CMario * mario;
	DWORD timeAttackDelay;
	DWORD timeAttacking =0;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void CreateBoomerang();
	float GetMarioRangeCurrent();
	BoomerangBrother(CMario* mario,float direction);
	~BoomerangBrother();
	void SetState(int state);
};

