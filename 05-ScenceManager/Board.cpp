#include "Board.h"

using namespace std;


Board::Board(float X, float Y)
{
	this->x = X;
	this->y = Y;
	Tex = CTextures::GetInstance()->Get(TEXID_BOARD);
	Sprite = new CSprite(0, 0, 0, 320, 120, Tex);
}
string Board::FillNumber(string s, UINT amountOfNumber) {
	while (s.size() < amountOfNumber)
		s = "0" + s;
	return s;
}


void Board::Render(CMario * mario, int RemainingTime) {
	Sprite->Draw(x, y);
	now = GetTickCount();
	//Map
	if (CGame::GetInstance()->GetScene() == SCENE_TEST) {
		font.Draw(x + 55, y + 12, FillNumber(std::to_string(1), 1));
	}
	//score
	font.Draw(x + 70, y + 20, FillNumber(std::to_string(mario->GetScore()), 7)); 
	//coin
	font.Draw(x + 150, y + 12, FillNumber(std::to_string(mario->GetCoinCollect()), 2));
	//Time
	font.Draw(x + 141, y + 20, FillNumber(std::to_string(RemainingTime), 3));
	//Live
	font.Draw(x + 47, y + 20, FillNumber(std::to_string(3), 1));
	//Logo
	LPDIRECT3DTEXTURE9 TexPatch = CTextures::GetInstance()->Get(TEXID_FONT35);
	if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
		LPSPRITE SpriteTile = new CSprite(0, 8, 43, 24, 50, TexPatch);
		SpriteTile->Draw(x + 22, y + 20);
	}
	else {
		LPSPRITE SpriteTile = new CSprite(0, 8, 50, 24, 57, TexPatch);
		SpriteTile->Draw(x + 22, y + 20);
	}
	
}



Board::~Board()
{
}
