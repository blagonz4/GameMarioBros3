#include "Camera.h"

Camera::Camera(CMario* mario, CGame* game, Map* map) {
	this->mario = mario;
	this->game = game;
	this->map = map;
}

void Camera::Update(DWORD dt) {

	sw = game->GetScreenWidth();
	sh = game->GetScreenHeight();
	mw = map->GetMapWidth();
	mh = map->GetMapHeight();

	if (game->GetScene() == MAP1_1 || game->GetScene() == MAP1_3) {

		if (mario == NULL) return;

		mario->GetPosition(px, py);

		if (__cx <= px - DISTANCE_TO_MARIO_X)
			__cx = px - DISTANCE_TO_MARIO_X;

		if (mario->isFlying)
			isTurnOn = true;
		if (__cy > mh - sh && !mario->isFlying)
			isTurnOn = false;

		if (isTurnOn && py > CAMERA_ON_CLOUD) {
			if (__cy >= CAMERA_COORDINATE_Y - CAMERA_ON_CLOUD_FIX)
				__cy-= CAMERA_ON_CLOUD_SPEED;
		}		
		else if (py < CAMERA_ON_CLOUD)  __cy = 0;
		else __cy = CAMERA_COORDINATE_Y;

		if (__cy <= 0)
			__cy = 0;//khong cho len cao qua chieu cao game
		if (__cx < cxMin)
			__cx = cxMin;//khong cho qua ben trai dau map
		if (__cx > mw - sw - CAMERA_END_GAME_DIFF)
			__cx = mw - sw - CAMERA_END_GAME_DIFF;//khong cho qua ben phai cuoi map

		game->SetCamPos((int)__cx, (int)__cy);
		map->SetCamPos(__cx, (float)__cy);
	}
	else if (game->GetScene() == MAP1_1_1)
	{

		if (mario == NULL) return;

		mario->GetPosition(px, py);

		if (__cx <= px - DISTANCE_TO_MARIO_X)
			__cx = px - DISTANCE_TO_MARIO_X;
		__cy = (int)py - CAMERA_MAP1_1_1_Y;

		if (__cy <= 0)
			__cy = 0;//khong cho len cao qua chieu cao game			
		if (__cx > mw - sw)
			__cx = mw - sw;//khong cho qua ben phai cuoi map

		game->SetCamPos((int)__cx, (int)__cy);
		map->SetCamPos(__cx, (float)__cy);
		//map->SetCamPos((int)__cx, (int)__cy);

	}
	else if (game->GetScene() == MAP1_3_1) {
		if (mario == NULL) return;

		mario->GetPosition(px, py);

		sum_dt += dt;
		__cx = game->GetCamX();
		if (sum_dt > CAMERA_REFRESH_RATE) {
			sum_dt = 0;
			__cx++;
		}
			
		if (mario->isFlying)
			isTurnOn = true;
		if (__cy > mh - sh && !mario->isFlying)
			isTurnOn = false;

		if (isTurnOn && py > CAMERA_ON_CLOUD) {
			if (__cy >= CAMERA_COORDINATE_Y - CAMERA_ON_CLOUD_FIX)
				__cy -= CAMERA_ON_CLOUD_SPEED;
		}
		else if (py < CAMERA_ON_CLOUD)  __cy = 0;
		else __cy = CAMERA_COORDINATE_Y;
		if (__cy <= 0)
			__cy = 0;//khong cho len cao qua chieu cao game
		if (__cx < cxMin)
			__cx = cxMin;//khong cho qua ben trai dau map
		if (__cx > mw - sw - CAMERA_END_GAME_DIFF)
			__cx = mw - sw - CAMERA_END_GAME_DIFF;//khong cho qua ben phai cuoi map


		game->SetCamPos((int)__cx, (int)__cy);
		map->SetCamPos(__cx, (float)__cy);
		//map->SetCamPos((int)__cx, (int)__cy);
	}
	else if (game->GetScene() == WORLDMAP){

		if (mario == NULL) return;

		mario->GetPosition(px, py);

		__cx = -WORLD_MAP_CAM_POS_X;
		__cy = -WORLD_MAP_CAM_POS_Y;	

		game->SetCamPos((int)__cx, (int)__cy);
		map->SetCamPos(__cx, (float)__cy);
		//map->SetCamPos((int)__cx, (int)__cy);
}

}

