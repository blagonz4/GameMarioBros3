#pragma once
#include "GameObject.h"
#include "Sprites.h"
#include "Game.h"
#include "Textures.h"

class Font
{
public:
	LPDIRECT3DTEXTURE9 Tex;
	void Draw(float x, float y, const string &s);
	Font();
	~Font();
};

