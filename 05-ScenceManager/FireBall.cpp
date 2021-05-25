#include "FireBall.h"

FireBall::FireBall(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_FIRE_FROM_FILE));
}

void FireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	vy += FIRE_GRAVITY * dt;
	vx = nx * FIRE_SPEED*dt;

	if (CheckObjectInCamera(this)) {

	}



	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

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
		y += min_ty * dy + ny * 0.1f;
		if (ny != 0)
		{	
			vy = -FIRE_BOUNCE_SPEED_Y ;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CKoopas *>(e->obj)) 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				koopas->nx = this->nx;
				koopas->SetState(KOOPAS_STATE_DIE);
				isFinish = true;
			}
			else if (dynamic_cast<CGoomba *>(e->obj)) {
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				this->isFinish = true;
				goomba->nx = this->nx;
				goomba->SetState(GOOMBA_STATE_DIE);
			}
			else if (dynamic_cast<Pipe *>(e->obj)) {
				Pipe *pipe = dynamic_cast<Pipe *>(e->obj);
				if (e->nx != 0) 
					this->isFinish = true;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void FireBall::Render() {
	int ani = -1;
	if (nx > 0) ani = SHOOT_FIRE_RIGHT;
	else ani = SHOOT_FIRE_LEFT;
	animation_set->at(ani)->Render(x, y);
}

void FireBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + FIRE_BBOX_WIDTH;
	bottom = top + FIRE_BBOX_HEIGHT;
}
