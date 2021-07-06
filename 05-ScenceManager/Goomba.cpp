#include "Goomba.h"
#include "Platform.h"
#include "Pipe.h"
CGoomba::CGoomba(CMario* mario, float model, float direction)
{	
	this->model = model;
	nx = direction;
	this->mario = mario;
	if (this->model == 1)	this->Health = 1;
	else this->Health = 2;

	eType = Type::GOOMBA;
	objType = ObjectType::ENEMY;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	if (state != GOOMBA_STATE_DIE) {
		vx = nx * GOOMBA_WALKING_SPEED;
	}

	if (model == GOOMBA_MODEL_WING_BROWN) {
		if ( y >= DISTANCE_MARIO_FLY_THROUGH_SKY_Y)
			vy = -MARIO_GRAVITY * 5 ;
		else vy += MARIO_GRAVITY ;

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
			TurnAround();
			vy = MARIO_GRAVITY * 5 * dt;
		}
	}
	else vy += MARIO_GRAVITY * dt;

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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0 ) x+=dx;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			/*if (dynamic_cast<Platform *>(e->obj))
			{
				Platform *platform = dynamic_cast<Platform *>(e->obj);

				if (e->nx < 0) {
					this->nx = -1;
				}
				else if (e->nx > 0) {
					this->nx = 1;
				}
				else if (ny < 0 && Health == 2) vy = -GOOMBA_JUMPING_SPEED;
				else if (ny > 0) vy = 0;

			}
			else if (dynamic_cast<Pipe *>(e->obj))
			{
				Pipe *pipe = dynamic_cast<Pipe *>(e->obj);

				if (e->nx < 0) {
					this->nx = -1;
				}
				else if (e->nx > 0) {
					this->nx = 1;
				}

			}*/

			if (e->nx != 0) {
				/*vx *= -1;
				this->nx *= -1;*/				
				if (e->obj->GetType() == COLORBLOCK || e->obj->GetObjectType() == ENEMY) {
					x += dx;
				}
				if (e->obj->GetType() == PIPE || 
					e->obj->GetType() == PLATFORM ||
					e->obj->GetType() == QUESTIONBRICK ||
					e->obj->GetType() == GOLDBRICK) {
					this->nx *= -1;
					this->vy = 0;
					this->vx = 0;
					this->vx = nx * GOOMBA_WALKING_SPEED * dt;
				}
			}
			if (ny != 0 ) {
				if (model == GOOMBA_MODEL_WING_BROWN)	vy = 0;
				else if (Health == 2) {
					vy = -GOOMBA_JUMPING_SPEED;
					y += dy;
				}
				else vy = 0;
						
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
		break;
	}
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	if (model == GOOMBA_MODEL_NORMAL)
		right = x + GOOMBA_BBOX_WIDTH;
	else right = x + GOOMBA_WING_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else if (model == GOOMBA_MODEL_NORMAL || Health == 1)
		bottom = y + GOOMBA_BBOX_HEIGHT;
	else if (listPoop.size() == 4)
		bottom = y + GOOMBA_BBOX_HEIGHT +5;
	else bottom = y + GOOMBA_WING_BBOX_HEIGHT;

}

void CGoomba::TurnAround() {
	float px, py;
	mario->GetPosition(px, py);
	if (px - this->x >20)
		this->nx = 1;
	if (this->x - px  > 150)
		this->nx = -1;
}