#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"
#include "Boomerang.h"
class BoomerangBrother : public CGameObject
{
public:
	vector<LPGAMEOBJECT> ListBoomerang;
	CMario * mario;
	DWORD timeAttackDelay;
	DWORD timeAttacking =0;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void CreateBoomerang();
	float GetMarioRangeCurrent();
	BoomerangBrother(CMario* mario,float direction);
	~BoomerangBrother();
	void SetState(int state);
};

