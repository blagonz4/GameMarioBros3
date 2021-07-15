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

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects) {
	CGameObject::Update(dt, colliable_objects);

	CGame *game = CGame::GetInstance();
	CMario* mario = ((WorldMap*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL && game->IsKeyDown(DIK_S)) {
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (CheckAABB(mLeft, mTop, mRight, mBottom)) {
			CGame::GetInstance()->SwitchScene(this->GetSceneId());
		}
	}
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