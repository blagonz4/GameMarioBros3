#include "Poop.h"
#include "Leaf.h"


Poop::Poop(float X, float Y,CMario* mario)
{
	this->x = X; this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_POOP_FROM_TXT));
	eType = Type::POOP;
	this->mario = mario;
	limitLeft = X;
	limitRight = X + LEAF_MAX_RIGHT_X;
}

void Poop::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + POOP_BBOX;
	bottom = y + POOP_BBOX;
}
void Poop::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (!this->CheckObjectInCamera())
	{
		isFinish = true;
	}
	x += dx;
	y += dy;

	if (x <= limitLeft)
	{
		vy = LEAF_GRAVITY * dt;
		vx = LEAF_SPEED_X * dt;
	}
	if (x >= limitRight)
	{
		vy = LEAF_GRAVITY * dt;
		vx = -LEAF_SPEED_X * dt;
	}
	else vy = LEAF_GRAVITY/2 * dt;

	if (isAttach) {
		AttachToMario();
		if (timeDisappear > TIME_DISAPPEAR) {
			mario->limitJumpVelocity = MARIO_JUMP_SPEED_MAX;
			isAttach = false;
			isFinish = true;
		}		
	}

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

		if (nx != 0) x += dx;
		if (ny != 0) y += dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == MARIO) {
				CMario* mario = dynamic_cast<CMario*>(e->obj);
				if (mario->untouchable == 0) {	
					this->isAttach = true;
					mario->limitJumpVelocity = 0.05f;
					
				}
			}
		}
	}
}
void Poop::Render()
{
	animation_set->at(POOP_ANI)->Render(x, y);
}

void Poop::AttachToMario()
{
	this->x = mario->x;
	this->y = mario->y+12;
	timeDisappear += dt;
}

Poop::~Poop()
{
}
