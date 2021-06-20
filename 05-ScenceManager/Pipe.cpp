#include "Pipe.h"
Pipe::Pipe(float X, float Y,float model)
{
	this->x = X;
	this->y = Y;
	this->model = model;
	eType = Type::PIPE;
}

void Pipe::Render()
{
	int ani = -1;
	if (model == SHORT_PIPE)
		ani = ANI_SHORT_PIPE;
	else ani = ANI_MEDIUM_PIPE;
	animation_set->at(ani)->Render(x+8, y);

	//RenderBoundingBox();
}
void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = this->x;
	t = y;
	r = this->x + PIPE_BBOX_WIDTH;
	if (model == SHORT_PIPE)
	{
		b = y + PIPE_SHORT_BBOX_HEIGHT;
	}
	if (model == MEDIUM_PIPE)
	{
		b = y + PIPE_MEDIUM_BBOX_HEIGHT;
	}
}
Pipe::~Pipe()
{
}
