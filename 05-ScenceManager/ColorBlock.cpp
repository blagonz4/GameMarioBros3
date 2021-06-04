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
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void ColorBlock::Render()
{
	RenderBoundingBox();
}
void ColorBlock::SetState(int state) {

}


void ColorBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y;
}
