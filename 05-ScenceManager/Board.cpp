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
	Sprite->Draw(x+ HUD_POS_X, y);
	//Map
	font.Draw(x + MAP_POS_X, y + MAP_POS_Y, FillNumber(std::to_string(1), 1));
	//score
	font.Draw(x + SCORE_POS_X, y + SCORE_POS_Y, FillNumber(std::to_string(mario->GetScore()), 7));
	//coin
	font.Draw(x + COIN_POS_X, y + COIN_POS_Y, FillNumber(std::to_string(mario->GetCoinCollect()), 2));
	//Time
	font.Draw(x + TIME_POS_X, y + TIME_POS_Y, FillNumber(std::to_string(RemainingTime), 3));
	//Live
	font.Draw(x + LIVE_POS_X, y + LIVE_POS_Y, FillNumber(std::to_string(3), 1));
	//Logo
	LPDIRECT3DTEXTURE9 TexPatch = CTextures::GetInstance()->Get(TEXID_FONT35);
	LPSPRITE SpriteTileLogo = new CSprite(0, 8, 50, 24, 57, TexPatch);
	SpriteTileLogo->Draw(x + LOGO_POS_X, y + LOGO_POS_Y);
	//Power
	if (mario->isRunning || mario->isFlying)
	{
		//float marioSpeed = MARIO_RUNNING_MAXSPEED / 7;
		if (mario->RunningStacks >= 1)
		{
			DrawPowerBar(x + RUNNING_STACK_1_POS_X, y + RUNNING_STACK_POS_Y);
		}
		if (mario->RunningStacks >= 2)
		{
			DrawPowerBar(x + RUNNING_STACK_2_POS_X, y + RUNNING_STACK_POS_Y);
		}
		if (mario->RunningStacks >= 3)
		{
			DrawPowerBar(x + RUNNING_STACK_3_POS_X, y + RUNNING_STACK_POS_Y);
		}
		if (mario->RunningStacks >= 4)
		{
			DrawPowerBar(x + RUNNING_STACK_4_POS_X, y + RUNNING_STACK_POS_Y);
		}
		if (mario->RunningStacks >= 5)
		{
			DrawPowerBar(x + RUNNING_STACK_5_POS_X, y + RUNNING_STACK_POS_Y);
		}
		if (mario->RunningStacks >= 6)
		{
			DrawPowerBar(x + RUNNING_STACK_6_POS_X, y + RUNNING_STACK_POS_Y);
		}
		if (abs(mario->vx) == MARIO_RUNNING_MAXSPEED)
		{
			LPSPRITE SpriteTilePower = new CSprite(0, 12, 33, 27, 40, TexPatch);
			SpriteTilePower->Draw(x + RUNNING_STACK_7_POS_X, y + RUNNING_STACK_POS_Y);
		}
	}
	//Card
	vector<int> listCards = mario->listCards;
	for (int i = 0; i < listCards.size();i++) {
		LPDIRECT3DTEXTURE9 Tex = CTextures::GetInstance()->Get(TEXID_FONT35);
		if (listCards[i] == BOX_STATE_MUSHROOM) {
			LPSPRITE SpriteTile = new CSprite(64, 187, 33, 211, 61, Tex);
			SpriteTile->Draw(x + CARD_POS_X + (CARD_DISTANCE * i), y + CARD_POS_Y);
		}
		else if (listCards[i] == BOX_STATE_FLOWER) {
			LPSPRITE SpriteTile = new CSprite(65, 211, 33, 235, 61, Tex);
			SpriteTile->Draw(x + CARD_POS_X + (CARD_DISTANCE * i), y + CARD_POS_Y);
		}
		else if (listCards[i] == BOX_STATE_STAR) {
			LPSPRITE SpriteTile = new CSprite(66, 235, 33, 259, 61, Tex);
			SpriteTile->Draw(x + CARD_POS_X + (CARD_DISTANCE * i), y + CARD_POS_Y);
		}
	}
}
	

void Board::DrawPowerBar(float X, float Y) {
	LPDIRECT3DTEXTURE9 TexPatch = CTextures::GetInstance()->Get(TEXID_FONT35);
	LPSPRITE SpriteTile = new CSprite(0, 3, 33, 11, 40, TexPatch);
	SpriteTile->Draw(X-1, Y);
}

Board::~Board()
{
}
