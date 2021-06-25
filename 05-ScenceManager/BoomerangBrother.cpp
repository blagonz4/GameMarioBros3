#include "BoomerangBrother.h"



BoomerangBrother::BoomerangBrother(CMario* mario,float direction)
{
	this->nx = direction;
	eType = Type::BOOMERANGBROTHER;
	objType = ObjectType::ENEMY;
	SetState(BOOM_BROTHER_STATE_WALKING);
	this->mario = mario;
}

void BoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += MARIO_GRAVITY * dt;
	if (state == BOOM_BROTHER_STATE_ATTACK)		vx = 0;
	if (state == BOOM_BROTHER_STATE_WALKING)	vx = nx * 0.001f *dt;
	if (this->CheckObjectInCamera(this)) {
		timeAttacking += dt;
		if (timeAttacking > TIME_BB_ATTACK)
		{
			timeAttacking = 0;
			SetState(BOOM_BROTHER_STATE_ATTACK);
			if (ListBoomerang.size() < 2)
			{
				CreateBoomerang();
			}
			else SetState(BOOM_BROTHER_STATE_WALKING);
		}
	}
		

	for (size_t i = 0; i < ListBoomerang.size(); i++)
	{
		ListBoomerang[i]->Update(dt, coObjects);
		if (!CheckObjectInCamera(ListBoomerang.at(i)) || ListBoomerang.at(i)->isFinish) {
			ListBoomerang.erase(ListBoomerang.begin() + i);
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != BOOM_BROTHER_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

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
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void BoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case BOOM_BROTHER_STATE_ATTACK:
	case BOOM_BROTHER_STATE_WALKING:
		break;
	case BOOM_BROTHER_STATE_DIE:
		isFinish = true;
		break;
	}

}
void BoomerangBrother::Render()
{
	if (isFinish)
		return;
	int marioRange = GetMarioRangeCurrent();
	int ani = -1;

	if (marioRange >0)
		ani = BOOM_BROTHER_ANI_RIGHT;
	else ani = BOOM_BROTHER_ANI_LEFT;
	animation_set->at(ani)->Render(x, y);
	for (size_t i = 0; i < ListBoomerang.size(); i++) {
		ListBoomerang.at(i)->Render();
	}
	RenderBoundingBox();
}
int BoomerangBrother::GetMarioRangeCurrent()
{
	if (mario->x < this->x)
	{
		this->nx = -1;
		return this->nx;
	}
	else
	{
		this->nx = 1;
		return this->nx;
	}
}
void BoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOM_BROTHER_BBOX_WIDTH;
	bottom = y + BOOM_BROTHER_BBOX_HEIGHT;

}
void BoomerangBrother::CreateBoomerang()
{
	int marioRange = GetMarioRangeCurrent();
	Boomerang* boomerang = new Boomerang(x + (marioRange*20), y-20, marioRange);
	ListBoomerang.push_back(boomerang);
}
BoomerangBrother::~BoomerangBrother()
{
}
