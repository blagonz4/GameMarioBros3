#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"
#include "Font.h"
#include "Textures.h"
#include <string>

#define HUD_POS_X	155
#define MAP_POS_X	65
#define MAP_POS_Y	12
#define SCORE_POS_X	82
#define SCORE_POS_Y	20
#define COIN_POS_X	162
#define COIN_POS_Y	12
#define TIME_POS_X	152
#define TIME_POS_Y	20
#define LIVE_POS_X	59
#define LIVE_POS_Y	20
#define RUNNING_STACK_1_POS_X	82
#define RUNNING_STACK_2_POS_X	90
#define RUNNING_STACK_3_POS_X	98
#define RUNNING_STACK_4_POS_X	106
#define RUNNING_STACK_5_POS_X	114
#define RUNNING_STACK_6_POS_X	122
#define RUNNING_STACK_7_POS_X	133
#define RUNNING_STACK_POS_Y		12
#define CARD_POS_X		197
#define CARD_POS_Y		5
#define CARD_DISTANCE	24
#define LOGO_POS_X	37
#define LOGO_POS_Y	20

class Board	
{
	float x, y;
	Font font;
	static Font* __instance;
	LPDIRECT3DTEXTURE9 Tex;
	LPSPRITE Sprite;
	DWORD timeDraw;
	DWORD now;
public:
	Board(float X, float Y);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	void Render(CMario* mario, int RemainingTime);
	void DrawPowerBar(float X, float Y);
	string FillNumber(string s, UINT amountOfNumber);
	~Board();
};

