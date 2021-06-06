#include "PSwitch.h"
#include "GoldBrick.h"
PSwitch::PSwitch(float X, float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::PSWITCH;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_PSWITCH_FROM_TXT));
	SetState(PSWITCH_STATE_NORMAL);
}
void PSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;
	right = x + PSWITCH_BBOX_WIDTH;
	top = y;
	bottom = y + PSWITCH_BBOX_HEIGHT;
	if (state == PSWITCH_STATE_USED)
		bottom = PSWITCH_BBOX_USED_HEIGHT;
}
void PSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
	
}
void PSwitch::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PSWITCH_STATE_NORMAL:
		break;
	case PSWITCH_STATE_USED:
		break;
	}
}
void PSwitch::Render()
{
	int ani = PSWITCH_ANI;
	if (state == PSWITCH_STATE_USED)
		ani = PSWITCH_ANI_USED;
	animation_set->at(ani)->Render(x, y);
}
PSwitch::~PSwitch()
{
}
