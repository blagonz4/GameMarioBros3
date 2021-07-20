#pragma once
#include "GameObject.h"
#include "define.h"

#define MUSIC_BRICK_SPEED_UP	0.15f
#define MUSIC_BRICK_MIN_Y		12
#define MUSIC_BRICK_POS_BOUND	2.f
#define MUSIC_BRICK_GRAVITY		0.02f
#define MUSIC_BRICK_BBOX_WIDTH	16
#define MUSIC_BRICK_BBOX_HEIGHT	16
class MusicBrick : public CGameObject
{
	float model;
public:
	bool boundDown = false;
	float startY; float minY; float maxY; bool isHidden = false;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	MusicBrick(float X, float Y,float model);
	void SetModel(float model) { this->model = model; }
	float GetModel() { return model; }
	~MusicBrick();
};

