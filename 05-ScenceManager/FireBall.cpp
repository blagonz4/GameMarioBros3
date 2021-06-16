#include "FireBall.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Scence.h"

FireBall::FireBall(float X, float Y,float defineVx, float defineVy)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_FIRE_FROM_FILE));
	this->defineVx = defineVx;
	this->defineVy = defineVy;
	eType = Type::FIRE;
}

void FireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if ( defineVy == FIRE_GRAVITY )
		vy += defineVy * dt;
	else vy = defineVy * dt;
	vx = nx * defineVx*dt;

	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	coEvents.clear();
	//if (defineVy == FIRE_GRAVITY) //lua tu mario
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

		if (ny != 0 && defineVy == FIRE_GRAVITY)
		{	
			vy = -FIRE_BOUNCE_SPEED_Y ;
		}
		//
		if (defineVy != FIRE_GRAVITY) {
			if (nx != 0) vx =0;
			if (ny != 0) vy =0;
		}
		

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == KOOPAS) 
			{
				e->obj->nx = this->nx;
				e->obj->SetState(KOOPAS_STATE_DIE);
				this->isFinish = true;
			}
			else if (e->obj->GetType() == GOOMBA) {
				this->isFinish = true;
				e->obj->nx = this->nx;
				e->obj->SetState(GOOMBA_STATE_DIE);
				this->isFinish = true;
			}
			else if (e->obj->GetType() == BOOMERANGBROTHER)
			{
				e->obj->nx = this->nx;
				e->obj->SetState(BOOM_BROTHER_STATE_DIE);
				this->isFinish = true;
			}
			else if (e->obj->GetType() == PIPE ||
				e->obj->GetType() == GOLDBRICK ||
				e->obj->GetType() == QUESTIONBRICK||
				e->obj->GetType() == MUSICBRICK) {
				if (e->nx != 0) 
					this->isFinish = true;
			}
			
			else if (e->obj->GetType() == MARIO) {
				if (e->obj->untouchable == 0) {
					
					if (e->obj->level > MARIO_LEVEL_SMALL)
					{
						
						e->obj->level = MARIO_LEVEL_SMALL;
						e->obj->StartUntouchable(5000);
					}
					else
						e->obj->SetState(MARIO_STATE_DIE);
				}
			}

			else if (e->obj->GetType() == PLATFORM || e->obj->GetType() == COLORBLOCK) {
				if (defineVy != FIRE_GRAVITY) {
					if ( e->nx != 0 )	this->isFinish = true;
					else { x += dx;	y += dy; }
				}	
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
