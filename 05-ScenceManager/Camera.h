#pragma once
#include "define.h"
#include "GameObject.h"
using namespace std;

class Camera {
	
	static Camera* __instance;
	float x = 0.0f;
	float y = 0.0f;
	bool followPlayer = true;
	int camWidth = SCREEN_WIDTH;
	int camHeight = SCREEN_HEIGHT;
	float vx;

public:
	static Camera* GetInstance();
	void SetPos(float x, float y) { this->x = x; this->y = y; }
	void GetPos(float& x, float& y) { x = this->x; y = this->y; }
	float getX() { return x; }
	float getY() { return y; }
	bool isFollow() { return followPlayer; }
	void setFollowPlayer(bool f) { this->followPlayer = f; }
	int getCamWidth() { return camWidth; }
	int getCamHeight() { return camHeight; }
	void Update(int mw, int mh);
	void UpdateToSwitchScene(int mw, int mh, DWORD dt, float sx, float sy, bool &done, bool);
};

