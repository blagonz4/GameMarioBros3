#include "Goomba.h"
#include "Platform.h"
#include "Pipe.h"
CGoomba::CGoomba(float model, float direction)
{	
	this->model = model;
	nx = direction;
	Health = (this->model == 1) ? 1 : 2;
	eType = Type::GOOMBA;
	objType = ObjectType::ENEMY;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	if (state != GOOMBA_STATE_DIE) {
		vx = nx * GOOMBA_WALKING_SPEED * dt;
	}
	vy += MARIO_GRAVITY * dt;
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

		//if (nx != 0 ) vx = 0;
		/*if (ny < 0 && Health == 2) {
			vy = -GOOMBA_JUMPING_SPEED;
		}
		else if (ny != 0) vy = 0;*/

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
				vx *= -1;
				this->nx *= -1;				
				if (e->obj->GetType() == COLORBLOCK || e->obj->GetObjectType() == ENEMY) {
					x += dx;
				}
			}
			if (ny != 0 ) {
				if (Health == 2) {
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
		if (model == 1)
			ani = GOOMBA_ANI_DIE;
		else ani = GOOMBA_ANI_WING_DIE;
	}
	else {
		if (model == 2) {
			if (Health == 2) {
				if (vy > 0)
					ani = GOOMBA_ANI_WING_FALLING;
				else if (vy < 0)
					ani = GOOMBA_ANI_WING_JUMPING;
				else ani = GOOMBA_ANI_WING_WALKING;
			}			
			else ani = GOOMBA_ANI_WING_WALKING_WITH_OUT_WING;
		}
		else ani = GOOMBA_ANI_WALKING;
	}

	animation_set->at(ani)->Render(x,y);

	RenderBoundingBox();
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
	if (model == 1)
		right = x + GOOMBA_BBOX_WIDTH;
	else right = x + GOOMBA_WING_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else if (model == 1 || Health == 1)
		bottom = y + GOOMBA_BBOX_HEIGHT;
	else bottom = y + GOOMBA_WING_BBOX_HEIGHT;

}
