#pragma once
#include "GameObject.h"
#include "define.h"
#include "Mario.h"
#include "Font.h"
#include "Textures.h"
#include <string>

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

