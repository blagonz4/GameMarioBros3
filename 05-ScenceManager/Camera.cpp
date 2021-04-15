//#include "Camera.h"
//#include <cstddef>
////#include "Player.h"
//#include "Game.h"
//#include "Utils.h"
//
//
//Camera* Camera::__instance = NULL;
//
//Camera* Camera::GetInstance() {
//	if (__instance == NULL) __instance = new Camera();
//	return __instance;
//}
//
//void Camera::UpdateToSwitchScene(int mw, int mh, DWORD dt, float sx, float sy, bool &done, bool toLeft)
//{
//	x += vx * dt;
//	y = sy / 2 - 30;
//	if (toLeft)
//	{
//		vx = 0.15f;
//		if (x > mw + 18 - SCREEN_WIDTH)
//		{
//			x = mw + 18 - SCREEN_WIDTH;
//			done = true;
//		}
//		if (y > mh + 38 - SCREEN_HEIGHT)
//			y = mh + 38 - SCREEN_HEIGHT;
//	}
//	else
//	{
//		vx = -0.15f;
//		if (x < 0)
//		{
//			x = 0;
//			done = true;
//		}
//		if (y < 0) y = 0;
//		if (y > mh + 38 - SCREEN_HEIGHT)
//			y = mh + 38 - SCREEN_HEIGHT;
//
//	}
//}
//void Camera::Update(int mw, int mh)
//{
//	Player* player = Player::GetInstance();
//	if (player->isSwitching()) return;
//
//	float cx, cy;
//	player->GetPosition(cx, cy);
//
//	CGame* game = CGame::GetInstance();
//	cx -= game->GetScreenWidth() / 2;
//	cy -= game->GetScreenHeight() / 2;
//	this->x = cx;
//	this->y = cy;
//
//	if (x > mw + 15 - SCREEN_WIDTH)
//		x = mw + 15 - SCREEN_WIDTH;
//	if (y > mh + 38 - SCREEN_HEIGHT)
//		y = mh + 38 - SCREEN_HEIGHT;
//
//	if (cx < 0)
//		x = 0;
//	if (cy < 0)
//		y = 0;
//}