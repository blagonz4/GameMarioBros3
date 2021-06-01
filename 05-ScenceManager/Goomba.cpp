#include "Goomba.h"
#include "Platform.h"
#include "Pipe.h"
CGoomba::CGoomba(int model, int direction)
{	
	this->model = model;
	nx = direction;
	Health = (this->model == 1) ? 1 : 2;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	vx = nx *GOOMBA_WALKING_SPEED * dt;
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
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

		if (nx != 0) vx = 0;
		if (ny != 0 && Health == 2) vy = GOOMBA_JUMPING_SPEED * dt;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Platform *>(e->obj))
			{
				Platform *platform = dynamic_cast<Platform *>(e->obj);

				if (e->nx < 0) {
					this->nx = -1; 
					vx = -GOOMBA_WALKING_SPEED * dt;
				}
				else if (e->nx > 0) {
					this->nx = 1;
					vx = GOOMBA_WALKING_SPEED * dt;
				}

			}

			if (dynamic_cast<Pipe *>(e->obj))
			{
				Pipe *pipe = dynamic_cast<Pipe *>(e->obj);

				if (e->nx < 0) {
					this->nx = -1;
					vx = -GOOMBA_WALKING_SPEED * dt;
				}
				else if (e->nx > 0) {
					this->nx = 1;
					vx = GOOMBA_WALKING_SPEED * dt;
				}

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
			if (vy > 0)
				ani = GOOMBA_ANI_WING_FALLING;
			else if (vy < 0)	
					ani = GOOMBA_ANI_WING_JUMPING;
			else if (Health == 2 )	
					ani = GOOMBA_ANI_WING_WALKING;
			else ani = GOOMBA_ANI_WING_WALKING_WITH_OUT_WING;
				

		}
		else ani = GOOMBA_ANI_WALKING;
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
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING: 
		break;
	}
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;

}
