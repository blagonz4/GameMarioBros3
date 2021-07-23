#include "ColorBlock.h"
#include "Mario.h"

ColorBlock::ColorBlock(float w, float h,CMario* mario)
{
	player = mario;
	this->width = w;
	this->height = h;
	eType = Type::COLORBLOCK;
}

void ColorBlock::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	float last_vy = player->vy;
	coEvents.clear();
	
}
void ColorBlock::Render()
{
	//RenderBoundingBox();
}
void ColorBlock::SetState(int state) {

}


void ColorBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y+1;
}
