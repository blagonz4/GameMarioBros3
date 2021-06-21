#include "Arrow.h"



Arrow::Arrow(float X, float Y)
{
	this->x = X; this->y = Y;
	eType = Type::ARROW;
}
void Arrow::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
}
void Arrow::Render() {
	animation_set->at(0)->Render(x, y);
}
Arrow::~Arrow()
{
}
