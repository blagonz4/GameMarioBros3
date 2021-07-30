#include "Poop.h"
#include "Mario.h"
#include "PlayScence.h"

Poop::Poop(float X, float Y)
{
	this->x = X; this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_POOP_FROM_TXT));
	eType = Type::POOP;
	limitLeft = X;
	limitRight = X + LEAF_MAX_RIGHT_X-10;
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
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!this->CheckObjectInCamera())
	{
		isFinish = true;
	}
	else isFinish = false;
	x += dx;
	y += dy;

	if (!isAttach) {
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
		else vy = LEAF_GRAVITY / 2 * dt;
	}

	if (isAttach) {
		this->x = mario->x;
		if (mario->level > MARIO_LEVEL_SMALL)
			this->y = mario->y + 12;
		else this->y = mario->y+5;

		mario->limitJumpVelocity = LIMIT_JUMP_VELOCITY;
		mario->limitRunSpeed = LIMIT_RUN_SPEED;
		timeDisappear += dt;
		if (timeDisappear > TIME_DISAPPEAR) {
			mario->limitJumpVelocity = MARIO_JUMP_SPEED_MAX;
			mario->limitRunSpeed = MARIO_RUNNING_MAXSPEED;
			isAttach = false;
			isFinish = true;
			timeDisappear = 0;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

}
void Poop::Render()
{
	animation_set->at(POOP_ANI)->Render(x, y);
}

void Poop::AttachToMario()
{

}

Poop::~Poop()
{
}
