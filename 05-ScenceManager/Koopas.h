#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"
#include "PlayScence.h"
//-----------------------------Koopas----------------------------
#define KOOPAS_DEFEND_TIME			5000
#define KOOPAS_DEFEND_HITBOX		16
#define KOOPAS_WALK_HITBOX_WIDTH	16
#define KOOPAS_WALK_HITBOX_HEIGHT	27
#define KOOPAS_TURN_BACK_SIZE		16
#define KOOPAS_MODEL_RED			1
#define KOOPAS_MODEL_GREEN			2
#define KOOPAS_MODEL_GREEN_WING		3

#define KOOPAS_WALKING_SPEED		0.0025f
#define	KOOPAS_FLY_SPEED			0.012f
#define KOOPAS_BALL_SPEED			0.01f
#define KOOPAS_BBOX_WIDTH			16
#define KOOPAS_BBOX_HEIGHT			26
#define KOOPAS_BBOX_HEIGHT_DIE		16

#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_DEFEND			200
#define KOOPAS_STATE_BALL			300
#define KOOPAS_STATE_REVIVE			400	//come back up side down
#define KOOPAS_STATE_DIE			500
#define KOOPAS_STATE_FLY			600
#define KOOPAS_STATE_UP_SIDE_DOWN	700

#define KOOPAS_ANI_RED_WALKING_LEFT			0
#define KOOPAS_ANI_RED_WALKING_RIGHT		1
#define KOOPAS_ANI_RED_DEFEND				2
#define KOOPAS_ANI_RED_BALL					3		
#define KOOPAS_ANI_GREEN_WALKING_LEFT		4
#define KOOPAS_ANI_GREEN_WALKING_RIGHT		5
#define KOOPAS_ANI_GREEN_DEFEND				6
#define KOOPAS_ANI_GREEN_BALL				7		
#define KOOPAS_ANI_GREEN_FLY_RIGHT			8
#define KOOPAS_ANI_GREEN_FLY_LEFT			9
#define KOOPAS_ANI_RED_DIE					10
#define KOOPAS_ANI_GREEN_DIE				11


class CKoopas : public CGameObject
{
	CMario* player;
	DWORD defend_start;
	int isDefend;
	void StartDefendTime() { isDefend = 1; defend_start = GetTickCount(); }

public:
	bool isBeingHeld;
	CKoopas(float &model, float &direction,CMario* mario);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

