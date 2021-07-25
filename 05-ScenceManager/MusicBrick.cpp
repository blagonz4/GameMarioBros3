#include "MusicBrick.h"

MusicBrick::MusicBrick(float X, float Y,float model)
{
	this->x = X;
	this->y = Y;
	SetModel(model);
	if (model == MUSIC_BRICK_MODEL_HIDDEN)
		this->isHidden = true;
	eType = Type::MUSICBRICK;
	startY = this->y;
	minY = y - MUSIC_BRICK_MIN_Y;
	maxY = y + MUSIC_BRICK_MIN_Y;
}

void MusicBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	y += dy;

	if (y <= minY)
	{
		vy = 0;
		y = startY;
	}

	if (y >= maxY )
	{
		vy = 0;
		y = startY;
		boundDown = false;
	}
}
void MusicBrick::Render()
{
	if (model == MUSIC_BRICK_MODEL_HIDDEN && !isHidden) {
		animation_set->at(0)->Render(x, y);
		RenderBoundingBox();
	}
	else if (!isHidden) animation_set->at(0)->Render(x, y);
}
void MusicBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MUSIC_BRICK_BBOX_WIDTH;
	b = y + MUSIC_BRICK_BBOX_HEIGHT;
}
MusicBrick::~MusicBrick()
{
}
