#include "MusicBrick.h"
#include "GoldBrick.h"

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
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
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

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
