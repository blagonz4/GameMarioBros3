#include "BackUp.h"

BackUp* BackUp::__instance = NULL;
BackUp* BackUp::GetInstance() {
	if (__instance == NULL)
		__instance = new BackUp();
	return __instance;
}

void BackUp::BackUpMario(CMario* mario) {
	if (mario != NULL) {
		life = mario->life;
		score = mario->GetScore();
		level = mario->GetLevel();
		money = mario->GetCoinCollect();
		listCards = mario->listCards;
		scene = CGame::GetInstance()->GetScene();
	}
}

void BackUp::LoadBackUp(CMario* mario) {
	if (mario != NULL) {
		mario->life = life;
		mario->score = score;
		mario->level = level;
		mario->coinCollect = money;
		mario->listCards = listCards;
	}
}