#include "QuestionBrick.h"
#include "GoldBrick.h"
QuestionBrick::QuestionBrick(float x, float y,float model)
{
	this->model = model;
	eType = Type::QUESTIONBRICK;
	Health = 1;
	startY = y;
	minY = y - QUESTION_BRICK_MIN_Y;
}

void QuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	CGameObject::Update(dt);
	y += dy;
	if (Health == 0)
	{
		if (y <= minY)
		{
			vy = QUESTION_BRICK_SPEED_UP * dt;
		}
		if (y >= startY)
		{
			y = startY;
			vy = 0;
		}
	}

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL && mario->level == MARIO_LEVEL_RACOON && mario->isTurningTail && Health == 1) {
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (mario->nx > 0 ) {
			if (CheckAABB(mLeft, mTop + TAIL_SIZE, mRight, mBottom)) {
				isUnbox = true;
				Health = 0;
				DebugOut(L"danh 1 lan \n");
				return;
			}				
		}
		else {
			if (CheckAABB(mLeft - 6, mTop + TAIL_SIZE, mRight, mBottom)) {
				isUnbox = true;
				Health = 0;
				DebugOut(L"danh 1 lan \n");
				return;
			}				
		}
	}
}
void QuestionBrick::Render()
{
	int ani = -1;
	if (Health == 1) ani = QUESTION_BRICK_ANI_BOX;
	else ani = QUESTION_BRICK_ANI_EMPTY;

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}
void QuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + QUESTION_BRICK_BBOX_WIDTH;
	b = y + QUESTION_BRICK_BBOX_HEIGHT;
}


QuestionBrick::~QuestionBrick()
{
}
