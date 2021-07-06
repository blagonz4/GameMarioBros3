#include "Boomerang.h"


Boomerang::Boomerang(float X, float Y, float dir, LPGAMEOBJECT bb)
{
	this->x = X; this->y = Y;
	this->startY = Y;
	eType = Type::BOOMERANG;
	this->nx = dir;
	this->boomerangbrother = bb;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_BOOMERANG_FROM_TXT));
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//if (this->boomerangbrother->isFinish || !this->CheckObjectInCamera()) {
	//	this->isFinish = true;
	//	return;
	//}

	if (startY - y < BOOMERANG_FLY_MIN_HEIGHT && !isTurning) {
		vx = nx * BOOMERANG_SPEED_X * dt;
		vy = -BOOMERANG_SPEED_Y *dt;
		isTurning = true;
	}
	if (startY - y > BOOMERANG_FLY_MIN_HEIGHT)
	{
		vx = nx * BOOMERANG_SPEED_X * dt;
		vy = BOOMERANG_SPEED_Y *dt;
	}
	if (isComingBack)
		vx = nx * BOOMERANG_SPEED_X *dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
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

		if (nx != 0) x+=dx;
		if (ny != 0) y+=dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == PLATFORM) {
				this->isComingBack = true;
				this->nx *= -1; vx = 0; vy = 0;
			}
			else if (e->obj->GetType() == COLORBLOCK || 
							e->obj->GetType() == GOLDBRICK ||
							e->obj->GetType() == QUESTIONBRICK ||
							e->obj->GetType() == MUSICBRICK ||
							e->obj->GetType() == KOOPAS || 
							e->obj->GetType() == GOOMBA ) {
					this->x += dx; 
					this->y += dy;
			}
			else if (e->obj->GetType() == MARIO) {
				if (e->obj->untouchable == 0) {
					if (e->obj->level > MARIO_LEVEL_SMALL)
					{

						e->obj->level = MARIO_LEVEL_SMALL;
						e->obj->StartUntouchable(TIME_UNTOUCHABLE_LONG);
					}
					else
						e->obj->SetState(MARIO_STATE_DIE);
					this->x += dx;
				}
			}
			else if (e->obj->GetType() == BOOMERANGBROTHER) {
				this->isFinish = true;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Boomerang::Render()
{
	int ani = BOOMERANG_ANI;
	animation_set->at(ani)->Render(x, y);
}
void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDTH;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}
Boomerang::~Boomerang()
{
}
