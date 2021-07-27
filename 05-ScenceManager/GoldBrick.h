#pragma once
#include "GameObject.h"
#include "define.h"
#include "PlayScence.h"

#define QUESTION_BRICK_BBOX_WIDTH			16
#define QUESTION_BRICK_BBOX_HEIGHT			16

#define QUESTION_BRICK_ANI_BOX				0
#define QUESTION_BRICK_ANI_EMPTY			1
#define	QUESTION_BRICK_ANI_POWER_UP			2

#define QUESTION_BRICK_MIN_Y				20
#define QUESTION_BRICK_SPEED_UP				0.18f

#define QUESTION_BRICK_MODEL_COIN			1
#define QUESTION_BRICK_MODEL_POWER_UP		2
#define QUESTION_BRICK_MODEL_ITEM			3
#define QUESTION_BRICK_MODEL_MANY_COIN		4
class GoldBrick : public CGameObject
{
	DWORD timeTransform;
public:
	float model;
	int isUnbox;
	float startX, startY, minY;
	GoldBrick(float x, float y, float model);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	~GoldBrick();
};

