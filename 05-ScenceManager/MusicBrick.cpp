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
	minY = y - QUESTION_BRICK_MIN_Y;
	maxY = y + QUESTION_BRICK_MIN_Y+5;
}

void MusicBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;

	if (y <= minY)
	{
		vy = 0;
		y = startY;
	}

	if (y >= maxY)
	{
		vy = 0;
		vy = -QUESTION_BRICK_SPEED_UP;
	}

}
void MusicBrick::Render()
{
	int alpha=100;
	if (model == MUSIC_BRICK_MODEL_HIDDEN && !isHidden)
		animation_set->at(0)->Render(x, y,alpha);
	else if (!isHidden) animation_set->at(0)->Render(x, y);
}
void MusicBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + QUESTION_BRICK_BBOX_WIDTH;
	b = y + QUESTION_BRICK_BBOX_HEIGHT;
}

MusicBrick::~MusicBrick()
{
}
