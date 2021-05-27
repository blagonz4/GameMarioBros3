#pragma once
#include "define.h"
#include "Game.h"
#include "Mario.h"
#include "Map.h"

#define DISTANCE_TO_MARIO_X					120
#define DISTANCE_TO_MARIO_Y					100
#define DISTANCE_MARIO_FLY_THROUGH_SKY_Y	280
#define CAMERA_COORDINATE_Y					250
#define CAMERA_ROOT_Y						0

using namespace std;

class Camera{
public:
	float cx,__cx, px, py;
	float __cy = CAMERA_COORDINATE_Y;
	CMario* mario;
	CGame* game = CGame::GetInstance();
	Map* map;
	Camera(CMario* mario,CGame* game, Map* map);
	virtual void Update(DWORD dt);
};

