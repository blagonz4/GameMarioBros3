#pragma once
#include "define.h"
#include "Game.h"
#include "Mario.h"
#include "Map.h"

using namespace std;

class Camera{
public:
	float cxMin,__cx, px, py;
	float cxMax = 2500;
	float __cy = CAMERA_COORDINATE_Y;
	CMario* mario;
	CGame* game = CGame::GetInstance();
	Map* map;
	Camera(CMario* mario,CGame* game, Map* map);
	virtual void Update(DWORD dt);
};

