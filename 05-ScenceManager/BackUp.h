#pragma once
#include "Mario.h"
#include "PlayScence.h"

class BackUp
{
public :
	static BackUp* __instance;
	int life = MARIO_DEFAULT_LIFE;
	int money = 0;
	DWORD time = 0;
	int score = 0;
	int level = MARIO_LEVEL_SMALL;
	int scene = 0;
	vector<int> listCards;
	static BackUp* GetInstance();
	void BackUpMario(CMario* mario);
	void LoadBackUp(CMario* mario);

};

