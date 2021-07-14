#pragma once
#include "GameObject.h"
#include "define.h"
#include "PlayScence.h"
class QuestionBrick : public CGameObject
{
	float startY;
	float minY; 
public:
	float model;
	bool isUnbox;
	QuestionBrick(float x, float y,float Model);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetModel() { return model; };
	~QuestionBrick();
};

