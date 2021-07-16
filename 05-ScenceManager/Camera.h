#pragma once
#include "define.h"
#include "Game.h"
#include "Mario.h"
#include "Map.h"

using namespace std;

class Camera{
public:
	bool isTurnOn = false;
	float cxMin,__cx, px, py;
	int sw, sh; float mw, mh;
	int cxMax = 2500;
	int __cy = CAMERA_COORDINATE_Y;
	CMario* mario;
	CGame* game = CGame::GetInstance();
	Map* map;
	Camera(CMario* mario,CGame* game, Map* map);
	virtual void Update(DWORD dt);
};

