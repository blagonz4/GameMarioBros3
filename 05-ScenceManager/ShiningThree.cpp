#include "ShiningThree.h"

ShiningThree::ShiningThree(float X, float Y)
{
	this->x = X; this->y = Y;
	eType = Type::ARROW;
}
void ShiningThree::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
}
void ShiningThree::Render() {
	animation_set->at(0)->Render(x, y);
}
ShiningThree::~ShiningThree()
{
}
