#include "Coin.h"

Coin::Coin()
{
	eType = Type::COIN;
}

void Coin::Render()
{
	int ani = COIN_ANI;
	if (isFinish)
		return;
	animation_set->at(ani)->Render(x, y);
}
void Coin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + COIN_WIDTH;
	bottom = y + COIN_HEIGHT;
	if (isFinish)
		left = top = right = bottom = 0;
}

Coin::~Coin()
{
}
