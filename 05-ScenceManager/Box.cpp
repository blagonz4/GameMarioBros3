#include "Box.h"

Box::Box()
{
	eType = Type::BOX;
	if (!isUnbox)	
		SetState((rand() % 3 + 1) * 100);
}

void Box::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOX_BBOX_WIDTH;
	bottom = y + BOX_BBOX_HEIGHT;
}

void Box::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
	y += dy;

	if (!isUnbox) {
		int stateBox = (rand() % 3 + 1) * 100;
		SetState(stateBox);
	}
	if(isUnbox)
		DebugOut(L"stateUnbox: %d \n", stateUnbox);
}

void Box::SetState(int state)
{
	CGameObject::SetState(state);

}
void Box::Render()
{

	int ani = -1;
	if (state == BOX_STATE_MUSHROOM)
		ani = BOX_ANI_MUSHROOM;
	if (state == BOX_STATE_FLOWER)
		ani = BOX_ANI_FLOWER;
	if (state == BOX_STATE_STAR)
		ani = BOX_ANI_STAR;
	if (state == BOX_STATE_MUSHROOM_UP)
		ani = BOX_ANI_MUSHROOM_UP;
	if (state == BOX_STATE_FLOWER_UP)
		ani = BOX_ANI_FLOWER_UP;
	if (state == BOX_STATE_STAR_UP)
		ani = BOX_ANI_STAR_UP;

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}
Box::~Box()
{
}
