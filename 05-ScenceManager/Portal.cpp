#include "Portal.h"


CPortal::CPortal(float l, float t,float r, float b, float scene_id )
{
	this->scene_id = scene_id;
	this->x = l;
	this->y = t;
	width = r - l + 1;
	height = b - t + 1;
	eType = Type::PORTAL;
	isFinish = false;
}

void CPortal::Render()
{
	//RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;
}