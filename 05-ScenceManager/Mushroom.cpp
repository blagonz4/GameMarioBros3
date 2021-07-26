#include "Mushroom.h"
#include "PlayScence.h"
#include "Mario.h"

Mushroom::Mushroom(float X, float Y, float model)
{
	this->model = model;
	this->x = X;
	this->y = Y;
	this->nx = 1;
	minY = Y - MUSHROOM_BBOX_HEIGHT;
	SetState(MUSHROOM_STATE_APPEAR);

	if (this->model == MUSHROOM_MODEL_RED) eType = Type::MUSHROOM_POWER;
	else  eType = Type::MUSHROOM_1_UP;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_MUSHROOM_FROM_TXT));
}

void Mushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MUSHROOM_BBOX_WIDTH;
	bottom = y + MUSHROOM_BBOX_HEIGHT;
}
void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
	CGameObject::Update(dt);

	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (!CheckObjectInCamera())
	{
		this->isFinish = true;
	}
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL && mario->level == MARIO_LEVEL_RACOON && mario->isTurningTail) {
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (CheckAABB(mLeft, mTop, mRight, mBottom)) {
			this->isFinish = true;
			mario->ShowEffectPoint(this, POINT_EFFECT_MODEL_1K);
			mario->PlusScore(1000);
			if (this->GetType() == MUSHROOM_POWER) {
				mario->y -= 20;
				if (mario->level < MARIO_LEVEL_BIG)
					mario->SetLevel(MARIO_LEVEL_BIG);
			}
		}
	}
	if (model == 1) {
		if (state == MUSHROOM_STATE_APPEAR)
			vy = -MUSHROOM_SPEED_APPEAR_Y;
		if (y <= minY)
		{
			isMoving = true;
		}
		if (isMoving)
		{
			vx = nx * MUSHROOM_SPEED * dt;
			vy = MUSHROOM_GRAVITY * dt;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(listObject, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.4f;
		if (ny < 0)
		{
			vy = 0;
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (e->obj->GetType() == COLORBLOCK)
			{
				if (e->nx != 0)
				{
					x += dx;
				}
			}
			if (e->obj->GetObjectType() == ENEMY)
			{
				if (e->nx != 0)
				{
					x += dx;
				}
				if (e->ny != 0)
				{
					y += dy;
				}
			}
			if (e->obj->GetType() == PIPE || e->obj->GetType() == PLATFORM)
			{
				if (e->ny != 0)
					vy = 0;
				if (e->ny < 0)
					vy = 0;
				if (e->nx != 0)
				{
					if (ceil(mBottom) != oTop)
					{
						vx = -vx;
						this->nx = -this->nx;
					}
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (isFinish) return;
}
void Mushroom::SetState(int state)
{
	CGameObject::SetState(state);
}

void Mushroom::Render()
{
	int ani = -1;
	if (model == MUSHROOM_MODEL_RED)
	{
		ani = MUSHROOM_RED_ANI;
	}
	if (model == MUSHROOM_MODEL_GREEN)
	{
		ani = MUSHROOM_GREEN_ANI;
	}
	animation_set->at(ani)->Render(x, y);
}
Mushroom::~Mushroom()
{
}
