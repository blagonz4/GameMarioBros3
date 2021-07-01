#include "GameObject.h"
#include "define.h"
#define FLOOR_SPEED_Y 0.1f
#define FLOOR_HEIGHT 183
#define BACKGROUND_STATE_CURTAIN	0
#define BACKGROUND_STATE_BLACK		1
#define BACKGROUND_STATE_COLOR		2
#define BACKGROUND_SPEED_Y			0.005f
class Curtain : public CGameObject
{
	DWORD timeChange;
	float minY;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state) { this->state = state; }
	Curtain(float X, float Y);
	~Curtain();
};

