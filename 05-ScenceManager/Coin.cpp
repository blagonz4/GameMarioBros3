#include "Coin.h"
#include "PlayScence.h"

Coin::Coin()
{
	eType = Type::COIN;
}
void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isFinish) return;
	y += dy;
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (CheckAABB(mLeft, mTop, mRight, mBottom)) {
			/*mario->x = x0;
			mario->y = y0;*/
			this->isFinish = true;
		}
	}

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
