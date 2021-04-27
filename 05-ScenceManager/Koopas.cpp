#include "Koopas.h"

CKoopas::CKoopas(float &model, int &direction)
{
	model = model;
	direction = direction;
	isDefend = 0;
	SetState(KOOPAS_STATE_WALKING);
}


void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	vy += MARIO_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	// turn off collision when die 
	if (state != KOOPAS_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if (isDefend == 0) {
		SetState(KOOPAS_STATE_WALKING);
	}
	if (GetTickCount() - defend_start > KOOPAS_DEFEND_TIME)
	{
		defend_start = 0;
		isDefend = 0;		
	}
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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CMario *>(e->obj)) // if e->obj is Goomba 
			{

			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (state == KOOPAS_STATE_WALKING) {
		if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
		else ani = KOOPAS_ANI_WALKING_LEFT;
	}
	else if (state == KOOPAS_STATE_DEFEND) {
		ani = KOOPAS_ANI_DEFEND;
	}
		
	else if (state == KOOPAS_STATE_BALL) {
		ani = KOOPAS_ANI_BALL;
	}
	if (state == KOOPAS_STATE_REVIVE) {
		ani = KOOPAS_ANI_REVIVE;
	}

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		if (nx >0 && isDefend == 0)
			vx = KOOPAS_WALKING_SPEED *dt ;
		if (nx < 0 && isDefend == 0)
			vx = -KOOPAS_WALKING_SPEED *dt;
		break;
	case KOOPAS_STATE_DEFEND:
		isDefend = 1;
		vx = 0;
		StartDefendTime();
		break;
	case KOOPAS_STATE_BALL:
		if (nx>0)
			vx = KOOPAS_BALL_SPEED * dt;
		else vx = -KOOPAS_BALL_SPEED * dt;
		break;
	}

}
void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (state == KOOPAS_STATE_DEFEND || state == KOOPAS_STATE_BALL)
	{
		right = left + KOOPAS_DEFEND_HITBOX;
		bottom = top + KOOPAS_DEFEND_HITBOX;
	}
	else
	{
		right = x + KOOPAS_WALK_HITBOX_WIDTH;
		bottom = y + KOOPAS_WALK_HITBOX_HEIGHT;
	}
}