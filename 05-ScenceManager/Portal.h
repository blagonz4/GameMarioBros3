#pragma once

#include "GameObject.h"
#include "define.h"
#include "WorldMap.h"
class CPortal : public CGameObject
{
	float scene_id;	// target scene to switch to 

	float width;
	float height;
public:
	bool cgLeft, cgRight, cgUp, cgDown;
	CPortal(float l, float t,float r, float b, float scene_id);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	float GetSceneId() { return scene_id;  }
	void SetMove(bool cLeft, bool cUp, bool cRight, bool cDown)
	{
		cgLeft = cLeft;
		cgRight = cRight;
		cgUp = cUp;
		cgDown = cDown;
	};
	void GetMove(bool& cLeft, bool& cUp, bool& cRight, bool& cDown)
	{
		cLeft = cgLeft;
		cRight = cgRight;
		cUp = cgUp;
		cDown = cgDown;
	};
};