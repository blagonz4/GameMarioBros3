#include "Curtain.h"

Curtain::Curtain(float X, float Y)
{
	this->x = X; this->y = Y;
	eType = Type::CURTAIN;
	SetState(BACKGROUND_STATE_CURTAIN);
}
void Curtain::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
	timeChange += dt;

	if (timeChange > 2000 && timeChange < 5000) {
		this->vy = -BACKGROUND_SPEED_Y * dt;
	}
		
	else if (timeChange > 5000 && timeChange < 8000) {
		this->vy = BACKGROUND_SPEED_Y * dt;
		SetState(BACKGROUND_STATE_BLACK);
	}
	else if (timeChange > 8000 && timeChange < 13800) {
		this->vy = 0;
	}
	else if (timeChange > 13800) {
		SetState(BACKGROUND_STATE_COLOR);
	}
		

}
void Curtain::Render()
{
	int ani = -1;
	if (state == BACKGROUND_STATE_CURTAIN)
		ani = 0;
	if (state == BACKGROUND_STATE_BLACK)
		ani = 1;
	if (state == BACKGROUND_STATE_COLOR)
		ani = 2;
	animation_set->at(ani)->Render(x, y);
}

Curtain::~Curtain()
{
}

