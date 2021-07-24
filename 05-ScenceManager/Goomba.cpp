#include "Goomba.h"
CGoomba::CGoomba(CMario* mario, float model, float direction)
{	
	this->model = model;
	nx = direction;
	this->mario = mario;
	if (this->model == GOOMBA_MODEL_NORMAL)	this->Health = 1;
	else this->Health = 2;
	eType = Type::GOOMBA;
	objType = ObjectType::ENEMY;
	if (this->model == GOOMBA_MODEL_WING_BROWN) {
		SetState(GOOMBA_STATE_BROWN_FLYING);
		StartFlying();
	}
	else	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	vy += ay * dt;
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (vy < -GOOMBA_JUMP_SPEED && state == GOOMBA_STATE_RED_JUMPING)
	{
		vy = -GOOMBA_JUMP_SPEED;
		ay = GOOMBA_GRAVITY;
	}
	if (vy < -GOOMBA_HIGHJUMP_SPEED && state == GOOMBA_STATE_RED_HIGHJUMPING)
	{
		vy = -GOOMBA_HIGHJUMP_SPEED;
		ay = GOOMBA_GRAVITY;
	}
	if (model == GOOMBA_MODEL_WING && Health == 2)
	{
		if (GetTickCount() - walking_start >= GOOMBA_RED_TIME_WALKING && isWalking )
		{
			isWalking = false;
			jumping_stacks = 0;
			y -= GOOMBA_WING_BBOX_HEIGHT - GOOMBA_WING_BBOX_WIDTH;
			SetState(GOOMBA_STATE_RED_JUMPING);
		}	
	}
	if (model == GOOMBA_MODEL_WING_BROWN && Health == 2) {

		TurnAround();
		if (GetTickCount() - timeFlying >= TIME_FLYING && isFlying)
		{
			timeFlying = 0;
			isFlying = false;
			StartWalking();
		}
		if (GetTickCount() - walking_start >= TIME_WALKING && isWalking)
		{
			walking_start = 0;
			isWalking = false;
			StartFlying();
		}
		if (isWalking) {
			vy = GOOMBA_FLYING_SPEED;
			ay = GOOMBA_GRAVITY;
		}
		if (isFlying) {
			if (y <= DISTANCE_MARIO_FLY_THROUGH_SKY_Y) {
				vy = 0;
				ay = GOOMBA_GRAVITY;
			}
			if (y >= DISTANCE_MARIO_FLY_THROUGH_SKY_Y) {
				vy = -GOOMBA_FLYING_SPEED;
				ay = 0;
			}
			timeDropDelay += dt;
			if (timeDropDelay >= POOP_DELAY_DROP) {
				Poop* poop = new Poop(x, y + 10, mario);
				CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
				scene->TurnIntoUnit(poop);
				timeDropDelay = 0;
			}
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (state == GOOMBA_STATE_DIE) {
		timeToDie += dt;
		if (timeToDie > TIME_TO_DIE)
			isFinish = true;
	}

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
		// block 
		x0 = x;
		y0 = y;

		x = x0 + min_tx * dx + nx * 0.4f;
		y = y0 + min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (e->obj->GetType() == COLORBLOCK || e->obj->GetObjectType() == ENEMY) {
				x = x0 + dx;
				if (this->model == GOOMBA_MODEL_WING || this->model == GOOMBA_MODEL_WING_BROWN)
					y = y0 + dy;
				else y = y0;
			}
			if (e->obj->GetType() == PIPE ||
				e->obj->GetType() == QUESTIONBRICK ||
				e->obj->GetType() == GOLDBRICK ||
				e->obj->GetType() == BRICK||
				e->obj->GetType() == MUSICBRICK) {
					this->vx = -this->vx;
			}
			if (e->obj->GetType() == PLATFORM)
			{
				if (e->ny != 0)
				{
					vy = 0;
					if (e->ny < 0 && model == GOOMBA_MODEL_WING && state != GOOMBA_STATE_DIE)
					{
						if (!isWalking)
						{
							if (jumping_stacks == GOOMBA_RED_JUMPING_STACKS)
							{
								SetState(GOOMBA_STATE_RED_HIGHJUMPING);
								jumping_stacks = -1;
							}
							else
							{
								if (jumping_stacks == -1)
									SetState(GOOMBA_STATE_RED_WINGSWALKING);
								else
									SetState(GOOMBA_STATE_RED_JUMPING);
								jumping_stacks++;
							}
						}
						else
							ay = GOOMBA_GRAVITY;
					}
					else if (e->ny > 0)
						ay = GOOMBA_GRAVITY;
				}
				else {
					if (ceil(mBottom) != oTop)
					{
						vx = -vx;
						this->nx = -this->nx;
					}
				}
			}
			if (e->obj->GetType() == COIN) {
				x = x0 + dx;
				y = y0 + dy;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int ani = -1;
	if (model == GOOMBA_MODEL_NORMAL) {
		if (state == GOOMBA_STATE_WALKING)
			ani = GOOMBA_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE)
			ani = GOOMBA_ANI_DIE;
	}
	if (model == GOOMBA_MODEL_WING) {
		if (Health == 2) {
			if (vy > 0)
				ani = GOOMBA_ANI_WING_FALLING;
			else if (vy < 0)
				ani = GOOMBA_ANI_WING_JUMPING;
			else ani = GOOMBA_ANI_WING_WALKING;
		}
		else ani = GOOMBA_ANI_WING_WALKING_WITH_OUT_WING;
		if (state == GOOMBA_STATE_DIE)
			ani = GOOMBA_ANI_WING_DIE;
	}
	else if (model == GOOMBA_MODEL_WING_BROWN) {
		if (Health == 2) {
			if (isFlying)			
				ani = GOOMBA_ANI_WING_BROWN_FLYING;
			if (isWalking)
				ani = GOOMBA_ANI_WING_BROWN_WALKING_WITH_WING;
		}
		else ani = GOOMBA_ANI_WING_BROWN_WALKING_WITHOUT_WING;
		if (state == GOOMBA_STATE_DIE)
			ani = GOOMBA_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y);

}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_RED_JUMPING:
		ay = -GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_RED_HIGHJUMPING:
		ay = -GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_WALKING:
		vx = nx * GOOMBA_WALKING_SPEED;
		ay = GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_RED_WINGSWALKING:
		StartWalking();
		ay = GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_BROWN_FLYING:
		//StartFlying();
		break;
	case GOOMBA_STATE_BROWN_WALKING:
		//StartWalking();
		break;
	}

}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (model > GOOMBA_MODEL_NORMAL && Health == 2) {
		right = x + GOOMBA_WING_BBOX_WIDTH;
		if (state == GOOMBA_STATE_RED_HIGHJUMPING || isFlying)
			bottom = y + GOOMBA_WING_BBOX_HEIGHT;
		else bottom = y + GOOMBA_WING_BBOX_WALK_HEIGHT;
	}
	else if (model == GOOMBA_MODEL_NORMAL || Health == 1)
		bottom = y + GOOMBA_BBOX_HEIGHT;
	else if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
}

void CGoomba::TurnAround() {
	float px, py;
	mario->GetPosition(px, py);
	if (px - x > 100) {
		nx = 1;
		SetState(GOOMBA_STATE_WALKING);
	}
		
	if (x - px > 100) {
		nx = -1;
		SetState(GOOMBA_STATE_WALKING);
	}

}