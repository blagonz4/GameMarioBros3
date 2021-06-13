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
	while (s.size() > amountOfNumber)
		s = s.substr(0, amountOfNumber);
	while (s.size() < amountOfNumber)
		s = "0" + s;
	return s;
}


void Board::Render(CMario * mario, int RemainingTime) {
	Sprite->Draw(x, y);
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
		LPSPRITE SpriteTileLogo = new CSprite(0, 8, 43, 24, 50, TexPatch);
		SpriteTileLogo->Draw(x + 22, y + 20);
	}
	else {
		LPSPRITE SpriteTileLogo = new CSprite(0, 8, 50, 24, 57, TexPatch);
		SpriteTileLogo->Draw(x + 22, y + 20);
	}
	//Power
	if (mario->GetState() == MARIO_STATE_RUN_RIGHT ||
		mario->GetState() == MARIO_STATE_RUN_LEFT ||
		mario->isFlying)
	{
		float marioSpeed = MARIO_RUNNING_MAXSPEED / 7;
		if (abs(mario->vx) >= marioSpeed)
		{
			DrawPowerBar(x + 70, y + 12);
		}
		if (abs(mario->vx) >= marioSpeed * 2)
		{
			DrawPowerBar(x + 78, y + 12);
		}
		if (abs(mario->vx) >= marioSpeed * 3)
		{
			DrawPowerBar(x + 86, y + 12);
		}
		if (abs(mario->vx) >= marioSpeed * 4)
		{
			DrawPowerBar(x + 94, y + 12);
		}
		if (abs(mario->vx) >= marioSpeed * 5)
		{
			DrawPowerBar(x + 102, y + 12);
		}
		if (abs(mario->vx) >= marioSpeed * 6)
		{
			DrawPowerBar(x + 110, y + 12);
		}

		if (abs(mario->vx) >= marioSpeed * 6.5f)
		{
			//DrawPowerBar(x + 118, y + 12);
			LPSPRITE SpriteTilePower = new CSprite(0, 12, 33, 27, 40, TexPatch);
			SpriteTilePower->Draw(x + 118, y + 12);
		}
	}
	//Card
	if (mario->gotCard) {
		LPDIRECT3DTEXTURE9 Tex = CTextures::GetInstance()->Get(TEXID_FONT35);
		if (mario->gotCard == BOX_STATE_MUSHROOM) {
			LPSPRITE SpriteTile = new CSprite(64, 187, 33, 211, 61, Tex);
			SpriteTile->Draw(x+178, y + 5);
		}
		else if (mario->gotCard == BOX_STATE_FLOWER) {
			LPSPRITE SpriteTile = new CSprite(65, 211, 33, 235, 61, Tex);
			SpriteTile->Draw(x + 178, y+5);
		}
		else if (mario->gotCard == BOX_STATE_STAR) {
			LPSPRITE SpriteTile = new CSprite(66, 235, 33, 259, 61, Tex);
			SpriteTile->Draw(x + 178, y + 5);
		}
	}
}
	

void Board::DrawPowerBar(float X, float Y) {
	LPDIRECT3DTEXTURE9 TexPatch = CTextures::GetInstance()->Get(TEXID_FONT35);
	LPSPRITE SpriteTile = new CSprite(0, 3, 33, 11, 40, TexPatch);
	SpriteTile->Draw(X, Y);
}

Board::~Board()
{
}
