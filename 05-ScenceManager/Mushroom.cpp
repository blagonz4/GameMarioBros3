#include "Mushroom.h"


Mushroom::Mushroom(float X, float Y, float model)
{
	this->model = model;
	this->x = X;
	this->y = Y;
	this->nx = 1;
	minY = Y - MUSHROOM_BBOX_HEIGHT +8;
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
	//if (!CheckObjectInCamera(this))
	//{
	//	this->isFinish = true;
	//}
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
				if (e->nx != 0)
				{
					this->nx *= -1;
					vx *= -1;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
