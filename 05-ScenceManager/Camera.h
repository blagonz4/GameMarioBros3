#pragma once
#include "define.h"
#include "Game.h"
#include "Mario.h"
#include "Map.h"

#define WORLD_MAP_CAM_POS_X	20
#define WORLD_MAP_CAM_POS_Y	20

#define CAMERA_ON_CLOUD			190
#define CAMERA_ON_CLOUD_FIX		170
#define CAMERA_ON_CLOUD_SPEED	20
#define CAMERA_MAP1_1_1_Y		200
#define CAMERA_MAX				2500
using namespace std;

class Camera{
public:
	bool isTurnOn = false;
	float cxMin,__cx, px, py;
	int sw, sh; float mw, mh;
	int cxMax = CAMERA_MAX;
	int __cy = CAMERA_COORDINATE_Y;
	CMario* mario;
	CGame* game = CGame::GetInstance();
	Map* map;
	Camera(CMario* mario,CGame* game, Map* map);
	virtual void Update(DWORD dt);
};

