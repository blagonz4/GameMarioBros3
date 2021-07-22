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
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	vy += ay * dt;
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
	if (model == GOOMBA_MODEL_WING_BROWN) {
		if ( y >= DISTANCE_MARIO_FLY_THROUGH_SKY_Y)
			vy = -MARIO_GRAVITY * 3 * dt ;
		else vy += MARIO_GRAVITY ;
		TurnAround();
		if (listPoop.size() < 4) {
			timeDropDelay += dt;
			if (timeDropDelay >= POOP_DELAY_DROP) {
				Poop* poop = new Poop(x, y + 10,mario);
				//scene->TurnIntoUnit(poop);
				listPoop.push_back(poop);
				timeDropDelay = 0;
			}	
		}
		else {
			vy = MARIO_GRAVITY * 3 * dt;
		}
	}

	for (size_t i = 0; i < listPoop.size(); i++)
	{
		listPoop[i]->Update(dt, coObjects);
		if (!listPoop[i]->CheckObjectInCamera() || listPoop.at(i)->isFinish) {
			listPoop.erase(listPoop.begin() + i);
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
				if (e->nx != 0) {
					this->vx = -this->vx;
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
	if (state == GOOMBA_STATE_DIE) {
		if (model == GOOMBA_MODEL_NORMAL || model == GOOMBA_MODEL_WING_BROWN)
			ani = GOOMBA_ANI_DIE;
		else ani = GOOMBA_ANI_WING_DIE;
	}
	else {
		if (model == GOOMBA_MODEL_WING){
			if (Health == 2) {
				if (vy > 0)
					ani = GOOMBA_ANI_WING_FALLING;
				else if (vy < 0)
					ani = GOOMBA_ANI_WING_JUMPING;
				else ani = GOOMBA_ANI_WING_WALKING;
			}			
			else ani = GOOMBA_ANI_WING_WALKING_WITH_OUT_WING;
		}
		else if (model == GOOMBA_MODEL_WING_BROWN) {
			if (Health == 2) {
				if (vy > 0)
					ani = GOOMBA_ANI_WING_BROWN_FALLING;
				else if (vy < 0)
					ani = GOOMBA_ANI_WING_BROWN_JUMPING;
				else ani = GOOMBA_ANI_WING_BROWN_FALLING;
			}
			else ani = GOOMBA_ANI_WALKING;
		}
		else ani = GOOMBA_ANI_WALKING;
	}
	for (size_t i = 0; i < listPoop.size(); i++) {
		listPoop.at(i)->Render();
	}
	animation_set->at(ani)->Render(x,y);

	//RenderBoundingBox();
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
	case GOOMBA_STATE_RED_WINGSWALKING:
		StartWalking();
		ay = GOOMBA_GRAVITY;
		break;
	}

}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (model > GOOMBA_MODEL_NORMAL) {
		right = x + GOOMBA_WING_BBOX_WIDTH;
		if (listPoop.size() == 4)
			bottom = y + GOOMBA_WING_BBOX_WALK_HEIGHT;
		else if (state == GOOMBA_STATE_RED_WINGSWALKING)
				bottom = y + GOOMBA_WING_BBOX_WALK_HEIGHT;
			 else bottom = y + GOOMBA_WING_BBOX_HEIGHT;
	}
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else if (model == GOOMBA_MODEL_NORMAL || Health == 1)
		bottom = y + GOOMBA_BBOX_HEIGHT;

}

void CGoomba::TurnAround() {
	float px, py;
	mario->GetPosition(px, py);
	if (px - x > 50) {
		nx = 1;
		SetState(GOOMBA_STATE_WALKING);
	}
		
	if (x - px > 50) {
		nx = -1;
		SetState(GOOMBA_STATE_WALKING);
	}

}