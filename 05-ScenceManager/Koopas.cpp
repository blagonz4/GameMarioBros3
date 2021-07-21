#include "Koopas.h"
CKoopas::CKoopas(float model)
{
	this->model = model;
	nx = -1;
	SetState(KOOPAS_STATE_WALKING);
	eType = Type::KOOPAS;
	objType = ObjectType::ENEMY;
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	if (state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_SPINNING || state == KOOPAS_STATE_SHELL_UP)
	{
		bottom = y + KOOPAS_BBOX_SHELL_HEIGHT;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}
bool CKoopas::CalKillable(vector<LPGAMEOBJECT>* coObjects)
{
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == PLATFORM)
		{
			float mLeft, mTop, mRight, mBottom;
			coObjects->at(i)->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (CheckAABB(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom)))
				return true;
		}
	return false;
}
bool CKoopas::CalTurnable(LPGAMEOBJECT object, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CheckObjectInCamera())
		return false;
	for (UINT i = 0; i < coObjects->size(); i++)
		if ((coObjects->at(i)->GetType() == PLATFORM || (coObjects->at(i)->GetType() == COLORBLOCK))) {
			if (abs(coObjects->at(i)->y == object->y))
			{
				if (nx > 0)
					if (coObjects->at(i)->x > object->x && coObjects->at(i)->x - BRICK_BBOX_WIDTH < object->x + BRICK_BBOX_WIDTH)
						return false;
				if (nx < 0)
					if (coObjects->at(i)->x + BRICK_BBOX_WIDTH > object->x - BRICK_BBOX_WIDTH && coObjects->at(i)->x < object->x)
						return false;
			}
		}
	return true;
}
void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CMario* mario = {};

	mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (state != KOOPAS_STATE_DEATH)
	{
		if (GetTickCount() - shell_start >= KOOPAS_SHELL_TIME && shell_start != 0 && state != KOOPAS_STATE_SPINNING)
		{
			shell_start = 0;
			StartReviving();
		}
		if (GetTickCount() - reviving_start >= KOOPAS_REVIVE_TIME && reviving_start != 0 && state != KOOPAS_STATE_SPINNING && shell_start == 0)
		{
			reviving_start = 0;
			y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) + 1.0f;
			if (isBeingHeld)
			{
				isBeingHeld = false;
				mario->SetIsHolding(false);
			}
			SetState(KOOPAS_STATE_WALKING);
		}
	}
	// Simple fall down
	if (!isBeingHeld )
	{
		if (model == KOOPAS_GREEN_PARA)
			vy += KOOPAS_PARA_GRAVITY * dt;
		if (model == KOOPAS_RED || model == KOOPAS_GREEN) {
			vy += KOOPAS_GRAVITY * dt;
		}		
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != KOOPAS_STATE_DEATH)
		CalcPotentialCollisions(coObjects, coEvents);

	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (mario != NULL)
	{
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (mario->isTurningTail && mario->level == MARIO_LEVEL_RACOON)
		{
			mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (CheckAABB(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom)))
			{
				SetState(KOOPAS_STATE_SHELL_UP);
				if (model == KOOPAS_GREEN_PARA)
					model = KOOPAS_GREEN;
			}
		}
		else if (!mario->isTurningTail)
		{
			mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (CheckAABB(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom)))
			{
				if (!mario->isHolding && isBeingHeld)
				{
					isBeingHeld = false;
					if ((state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_SHELL_UP) && !isBeingHeld)
					{
						this->nx = mario->nx;
						this->SetState(KOOPAS_STATE_SPINNING);
						mario->StartKicking();
						if (isKillable)
						{
							this->SetState(KOOPAS_STATE_DEATH);
							mario->PlusScore(100);
						}
					}
				}
				else if (!isBeingHeld)
				{
					if (abs(oBottom - mTop) <= 1.0f && mario->y > y &&
						mario->untouchable == 0
						&& (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_SPINNING))
						mario->Attacked();
				}
			}
		}
		if (isBeingHeld)
		{
			y = mario->y + KOOPAS_BBOX_SHELL_HEIGHT / 2;
			float tmp = mario->vx;
			if (tmp < 0)
				tmp = -1;
			if (tmp > 0)
				tmp = 1;
			if (tmp == 0)
				tmp = mario->nx;

			x = mario->x + tmp * (MARIO_BIG_BBOX_WIDTH);
			if (mario->level == MARIO_LEVEL_SMALL)
			{
				if (tmp > 0)
					x = mario->x + tmp * (MARIO_SMALL_BBOX_WIDTH);
				else
					x = mario->x + tmp * (KOOPAS_BBOX_WIDTH);
				y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
			}
			isKillable = CalKillable(coObjects);
		}
	}
	// No collision occured, proceed normally
	if (coEvents.size() == 0 || isBeingHeld)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty;
		float nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x0 = x, y0 = y;
		x = x0 + min_tx * dx + nx * 0.4f;
		y = y0 + min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj != NULL)
				if (e->obj->isFinish == true)
					continue;
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas 
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->ny > 0)
					y = y0;
				if (koopas->state == KOOPAS_STATE_SPINNING)
				{
					if (model == KOOPAS_GREEN_PARA)
						model = KOOPAS_GREEN;

					this->SetState(KOOPAS_STATE_DEATH);
					mario->PlusScore(100);
				}
				else
				{
					x = x0;
					y = y0;
					if ((koopas->state == KOOPAS_STATE_SHELL_UP || koopas->state == KOOPAS_STATE_IN_SHELL)
						&& state == KOOPAS_STATE_WALKING)
					{
						x = x0 + dx;
						y = y0 + dy;
					}
					if (state == KOOPAS_STATE_WALKING && koopas->state == KOOPAS_STATE_WALKING)
					{
						x = x0 + dx;
						if (e->ny < 0)
							y = y0 + dy;
						this->vx = -this->vx;
						this->nx = -this->nx;
						koopas->vx = -koopas->vx;
						koopas->nx = -koopas->nx;
					}
				}
			}
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (goomba->GetState() != GOOMBA_STATE_DIE && (this->GetState() == KOOPAS_STATE_SPINNING || isBeingHeld))
				{
					mario->PlusScore(100);
					goomba->SetState(GOOMBA_STATE_DIE_BY_TAIL);
				}
				else
				{
					goomba->vx = -goomba->vx;
					goomba->nx = -goomba->nx;
					this->vx = -this->vx;
					this->nx = -this->nx;
				}
			}
			if (e->obj->GetType()== FIREPLANT)
			{
				mario->PlusScore(100);
				SetState(PIRANHAPLANT_STATE_DEATH);
			}
			if (e->obj->GetType() == PLATFORM)
			{
				Platform* object = dynamic_cast<Platform*>(e->obj);
				object->GetBoundingBox(oLeft, oTop, oRight, oBottom);
				if (e->ny != 0)
				{
					vy = 0;
					if (state == KOOPAS_STATE_SHELL_UP)
						vx = 0;
				}
				if (e->ny < 0)
				{
					if (model == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
					{
						if (this->nx > 0 && x >= e->obj->x + KOOPAS_TURN_DIFF)
						{
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = -1;
								vx = -KOOPAS_WALKING_SPEED;
							}
						}
						if (this->nx < 0 && x <= e->obj->x - KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = 1;
								vx = KOOPAS_WALKING_SPEED;
							}
					}
					if (model == KOOPAS_GREEN_PARA)
						vy = -KOOPAS_JUMP_SPEED;
				}
				if (e->nx != 0)
				{
					if (ceil(mBottom) != oTop)
					{
						vx = -vx;
						this->nx = -this->nx;
					}
				}
			}
			if (dynamic_cast<QuestionBrick*>(e->obj) && state == KOOPAS_STATE_SPINNING && e->nx != 0 && ceil(mBottom) != oTop)
			{
				QuestionBrick* qb = dynamic_cast<QuestionBrick*>(e->obj);
				qb->isUnbox = true;
			}
			if (dynamic_cast<GoldBrick*>(e->obj) && state == KOOPAS_STATE_SPINNING && e->nx != 0 && ceil(mBottom) != oTop)
			{
				GoldBrick* tmp = dynamic_cast<GoldBrick*>(e->obj);
				tmp->isFinish= true;
			}
			if (e->obj->GetType() == COLORBLOCK)
			{
				if (e->ny < 0)
				{
					vy = 0;
					if (state == KOOPAS_STATE_SHELL_UP)
						vx = 0;
					if (model == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
					{
						if (this->nx > 0 && x >= e->obj->x + KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = -1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
						if (this->nx < 0 && x <= e->obj->x - KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = 1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
					}
					if (model == KOOPAS_GREEN_PARA)
					{
						y = e->obj->y - KOOPAS_BBOX_HEIGHT;
						vy = -KOOPAS_JUMP_SPEED;
					}

				}
				else
				{
					if (e->nx != 0)
						x = x0 + dx;
					if (state == KOOPAS_STATE_SHELL_UP && e->ny > 0)
						y = y0 + dy;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}
void CKoopas::Render()
{

	int ani = -1;
	if (state == KOOPAS_STATE_SHELL_UP || state == KOOPAS_STATE_DEATH)
		ani = KOOPAS_ANI_GREEN_DEFEND;
	else if (state == KOOPAS_STATE_IN_SHELL)
		ani = KOOPAS_ANI_GREEN_DEFEND;
	else if (state == KOOPAS_STATE_SPINNING)
	{
		if (vx < 0)
			ani = KOOPAS_ANI_GREEN_BALL;
		else
			ani = KOOPAS_ANI_GREEN_BALL;
	}
	else
	{
		if (nx < 0)
			ani = KOOPAS_ANI_GREEN_WALKING_LEFT;
		else
			ani = KOOPAS_ANI_GREEN_WALKING_RIGHT;
	}
	if (model == KOOPAS_GREEN_PARA)
		if (nx < 0)
			ani = KOOPAS_ANI_GREEN_FLY_LEFT;
		else
			ani = KOOPAS_ANI_GREEN_FLY_LEFT;
	if (reviving_start != 0)
	{
		if (state == KOOPAS_STATE_IN_SHELL)
			ani = KOOPAS_ANI_GREEN_BALL;
		if (state == KOOPAS_STATE_SHELL_UP)
			ani = KOOPAS_ANI_GREEN_BALL;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}
void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SHELL_UP:
	{
		CMario* mario = {};
		mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		vy = -KOOPAS_SHELL_DEFLECT_SPEED;
		if (x <= mario->x)
			nx = -1;
		else
			nx = 1;
		vx = nx * KOOPAS_WALKING_SPEED;
		StartShell();
		break;
	}
	case KOOPAS_STATE_WALKING:
		if (model == KOOPAS_GREEN_PARA)
			vx = nx * KOOPAS_PARA_WALKING_SPEED;
		if (model == KOOPAS_RED || model == KOOPAS_GREEN)
			vx = nx * KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_SPINNING:
		if (nx > 0)
			vx = KOOPAS_WALKING_SPEED * 5;
		else
			vx = -KOOPAS_WALKING_SPEED * 5;
		break;
	case KOOPAS_STATE_IN_SHELL:
		vx = 0;
		StartShell();
		break;
	case KOOPAS_STATE_DEATH:
		vy = -KOOPAS_DIE_DEFLECT_SPEED;
		vx = 0;
		StartDying();
		break;
	}

}