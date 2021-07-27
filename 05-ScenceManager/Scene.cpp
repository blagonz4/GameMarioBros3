#include "Scene.h"
#include "PlayScence.h"
#include "Mario.h"

Scene::Scene(float model,float cgLeft, float cgTop, float cgRight, float cgBottom)
{
	this->model = model;
	this->cgLeft = cgLeft;
	this->cgUp = cgTop;
	this->cgRight = cgRight;
	this->cgDown = cgBottom;
	eType = Type::SCENE;
}

void Scene::Render()
{
	int ani = -1;
	switch ((int)model) {
	case SCENE_STATE_1:
		ani = SCENE_ANI_1;
		break;
	case SCENE_STATE_2:
		ani = SCENE_ANI_2;
		break;
	case SCENE_STATE_3:
		ani = SCENE_ANI_3;
		break;
	case SCENE_STATE_4:
		ani = SCENE_ANI_4;
		break;
	case SCENE_STATE_5:
		ani = SCENE_ANI_5;
		break;
	case SCENE_STATE_6:
		ani = SCENE_ANI_6;
		break;
	}
	if (ani != -1)
		animation_set->at(ani)->Render(x, y);
}
void Scene::Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects) {
	CGame *game = CGame::GetInstance();
	CMario* mario = ((WorldMap*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (CheckAABB(mLeft, mTop, mRight, mBottom)) {
			bool cl, cr, cu, cd;
			GetMove(cl, cu, cr, cd);
			mario->SetMove(cl, cu, cr, cd);
		}
	}
}

void Scene::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (eType != BUSH)
	{
		left = x + 2;
		top = y + 2;
		right = left + OBJECT_BBOX_WIDTH;
		bottom = top + OBJECT_BBOX_WIDTH;
	}
}

Scene::~Scene()
{
}
