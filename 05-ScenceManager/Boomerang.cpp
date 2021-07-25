#include "Boomerang.h"
#include "Mario.h"
#include "PlayScence.h"

CBoomerang::CBoomerang(float X, float Y, float dir)
{
	this->x = X; this->y = Y;
	this->startY = Y;
	eType = Type::BOOMERANG;
	this->nx = dir;
	SetState(BOOMERANG_STATE_IDLE);
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_BOOMERANG_FROM_TXT));
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (!isAppear)
		return;
	CGameObject::Update(dt);
	//update speed
	x += dx;
	y += dy;
	if (state_start != 0)
		state_start += dt;
	if (state == 1 && state_start >= BOOMERANG_CHANGE_STATE_TIME)
	{
		state_start = 1;
		SetState(state + 1);
	}
	if (state > BOOMERANG_STATE_1 && state <= BOOMERANG_STATE_6 && state_start >= BOOMERANG_CHANGE_STATE_TIME / BOOMERANG_DIFF)
	{
		state_start = 1;
		SetState(state + 1);
	}

}
void CBoomerang::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_STATE_IDLE:
		vx = vy = 0;
		break;
	case BOOMERANG_STATE_1:
		vx = nx * BOOMERANG_SPEED_X;
		vy = -BOOMERANG_SPEED_Y;
		StartChangState();
		break;
	case BOOMERANG_STATE_2:
		vy = 0;
		break;
	case BOOMERANG_STATE_3:
		vy = BOOMERANG_SPEED_Y;
		break;
	case BOOMERANG_STATE_4:
		vx = 0;
		vy = BOOMERANG_SPEED_Y * BOOMERANG_SPEED_DIFF;
		break;
	case BOOMERANG_STATE_5:
		vy = BOOMERANG_SPEED_Y;
		vx = -nx * BOOMERANG_SPEED_X;
		break;
	case BOOMERANG_STATE_6:
		vx = -nx * BOOMERANG_SPEED_X;
		vy = 0;
		break;
	}
}
void CBoomerang::Render()
{
	if (!isAppear)
		return;
	int ani = BOOMERANG_ANI;
	animation_set->at(0)->Render(x - BOOMERANG_DIFF, y - BOOMERANG_DIFF);
	//RenderBoundingBox();
}
void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDTH;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}
CBoomerang::~CBoomerang()
{
}
