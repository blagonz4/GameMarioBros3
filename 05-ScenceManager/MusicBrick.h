#pragma once
#include "GameObject.h"
#include "define.h"
class MusicBrick : public CGameObject
{
	float model;
public:
	float startY; float minY; float maxY; bool isHidden = false;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	MusicBrick(float X, float Y,float model);
	void SetModel(float model) { this->model = model; }
	float GetModel() { return model; }
	~MusicBrick();
};

