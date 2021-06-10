#include "Camera.h"

Camera::Camera(CMario* mario, CGame* game, Map* map) {
	this->mario = mario;
	this->game = game;
	this->map = map;
}

void Camera::Update(DWORD dt) {

	if (game->GetScene() == SCENE_TEST) {

		if (mario == NULL) return;

		mario->GetPosition(px, py);


		if (__cx <= px - DISTANCE_TO_MARIO_X)
			__cx = px - DISTANCE_TO_MARIO_X;
		

		if (py < DISTANCE_MARIO_FLY_THROUGH_SKY_Y)
			__cy = py - DISTANCE_TO_MARIO_Y;
		if (py > DISTANCE_MARIO_FLY_THROUGH_SKY_Y && py < CAMERA_COORDINATE_Y)
			__cy = py + DISTANCE_TO_MARIO_Y;

		if (__cy <= 0)
			__cy = 0;//khong cho len cao qua chieu cao game
		if (__cx < cx)
			__cx = cx;//khong cho qua ben trai dau map
		game->SetCamPos((int)__cx, (int)__cy);
		map->SetCamPos((int)__cx, (int)__cy);
		
	}

	if (game->GetScene() == WORLD1_1_1) {

		if (mario == NULL) return;

		mario->GetPosition(px, py);

		if (__cx <= px - DISTANCE_TO_MARIO_X)
			__cx = px - DISTANCE_TO_MARIO_X;
		__cy = py - 200;
		//if (py < DISTANCE_MARIO_FLY_THROUGH_SKY_Y)
		//	__cy = py - DISTANCE_TO_MARIO_Y;
		//if (py > DISTANCE_MARIO_FLY_THROUGH_SKY_Y && py < CAMERA_COORDINATE_Y)
		//	__cy = py + DISTANCE_TO_MARIO_Y;




		if (__cy <= 0)
			__cy = 0;//khong cho len cao qua chieu cao game
		if (__cx < cx)
			__cx = cx;//khong cho qua ben trai dau map
		game->SetCamPos((int)__cx, (int)__cy);
		map->SetCamPos((int)__cx, (int)__cy);

	}



}

