#include "BoomerangBrother.h"



BoomerangBrother::BoomerangBrother(float x, float y,float direction)
{
	this->nx = direction;
	this->start_x = x;
	eType = Type::BOOMERANGBROTHER;
	objType = ObjectType::ENEMY;
	SetState(BOOMERANG_BROTHER_STATE_FORWARD);
	this->mario = mario;
	StartReload();
	for (int i = 0; i < BOOMERANG_BROTHER_BOOMERANGS; i++)
	{
		CBoomerang* boomerang = new CBoomerang(x,y, direction);
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		ListBoomerang.push_back(boomerang);
		scene->TurnIntoUnit(boomerang);
	}
}

void BoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += MARIO_GRAVITY * dt;
	if (x < start_x)
		vx = BOOMERANG_BROTHER_SPEED;
	if (x > start_x + BOOMERANG_BROTHER_LIMIT_RANGE)
		vx = -BOOMERANG_BROTHER_SPEED;
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL && state != BOOMERANG_BROTHER_STATE_DIE){
		if (mario->level == MARIO_LEVEL_RACOON && mario->isTurningTail) {

			mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (CheckAABB(mLeft, mTop + TAIL_SIZE, mRight, mBottom))
				isFinish = true;
		}
		if (mario->x > x)
			nx = 1;
		else
			nx = -1;
		for (size_t i = 0; i < ListBoomerang.size(); i++)
			if (ListBoomerang[i]->GetState() == BOOMERANG_STATE_IDLE)
				ListBoomerang[i]->nx = nx;
	}

	for (size_t i = 0; i < ListBoomerang.size(); i++)
	{
		if (ListBoomerang[i]->state >= BOOMERANG_STATE_2)
		{
			ListBoomerang[i]->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (CheckAABB(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom)))
			{
				if (i == 1)
					StartReload();
				ListBoomerang[i]->SetState(BOOMERANG_STATE_IDLE);
				ListBoomerang[i]->isAppear = false;
			}
		}
	}
	if (reload_start != 0 && GetTickCount() - reload_start >= BOOMERANG_BROTHER_RELOAD_TIME
		&& abs(x - mario->x) <= BOOMERANG_BROTHER_ACTIVE_RANGE)
	{
		reload_start = 0;
		StartAim();
	}
	if (aim_start != 0 && GetTickCount() - aim_start >= BOOMERANG_BROTHER_AIM_TIME)
	{
		aim_start = 0;
		StartThrow();
		ListBoomerang[bmrIndex]->nx = nx;
		if (bmrIndex == 0)
			StartChange();
		ListBoomerang[bmrIndex++]->SetState(BOOMERANG_STATE_1);
		if (bmrIndex == BOOMERANG_BROTHER_BOOMERANGS)
			bmrIndex = 0;
	}
	if (aim_start > 0)
	{
		float bx = x + BOOMERANG_BBOX_WIDTH - nx * BOOMERANG_DIFF * 2;
		if (nx > 0)
			bx = x - nx * BOOMERANG_DIFF * 2;
		ListBoomerang[bmrIndex]->SetState(BOOMERANG_STATE_IDLE);
		ListBoomerang[bmrIndex]->SetPosition(bx, y);
		ListBoomerang[bmrIndex]->isAppear = true;
	}
	if (throw_start != 0 && GetTickCount() - throw_start >= BOOMERANG_BROTHER_THROW_TIME)
	{
		throw_start = 0;
	}
	if (change_start != 0 && GetTickCount() - change_start >= BOOMERANG_BROTHER_CHANGE_TIME
		&& abs(x - mario->x) <= BOOMERANG_BROTHER_ACTIVE_RANGE)
	{
		change_start = 0;
		StartAim();
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != BOOMERANG_BROTHER_STATE_DIE)
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
		float x0 = x, y0 = y;
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.4f;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (e->obj->GetType() == PLATFORM)
			{
				if (e->ny < 0)
					vy = 0;
				if (e->nx != 0)
				{
					if (ceil(mBottom) != oTop)
						vx = -vx;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void BoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_BROTHER_STATE_FORWARD:
		vx = BOOMERANG_BROTHER_SPEED;
		break;
	case BOOMERANG_BROTHER_STATE_DIE:
		vy = -BOOMERANG_BROTHER_DEFLECT_SPEED;
		break;
	}
}
void BoomerangBrother::Render()
{
	if (isFinish)
		return;
	int ani = -1;
	if (nx > 0)
	{
		if (aim_start != 0)
			ani = BOOMERANG_BROTHER_ANI_AIM_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_THROW_RIGHT;
	}
	else
	{
		if (aim_start != 0)
			ani = BOOMERANG_BROTHER_ANI_AIM_LEFT;
		else
			ani = BOOMERANG_BROTHER_ANI_THROW_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	for (size_t i = 0; i < ListBoomerang.size(); i++) {
		ListBoomerang.at(i)->Render();
	}
}
float BoomerangBrother::GetMarioRangeCurrent()
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
}
BoomerangBrother::~BoomerangBrother()
{
}
