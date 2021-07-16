#pragma once
#include <iostream>
#include <fstream>
#include "Game.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "define.h"
#include "Platform.h"
#include "FireBall.h"
#include "ColorBlock.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Map.h"
#include "Mario.h"
#include "Koopas.h"
#include "FireBall.h"
#include "Goomba.h"
#include "Pipe.h"
#include "FirePlant.h"
#include "Camera.h"
#include "QuestionBrick.h"
#include "GoldBrick.h"
#include "Coin.h"
#include "PSwitch.h"
#include "Leaf.h"
#include "Mushroom.h"
#include "EffectTailHit.h"
#include "EffectBrokenBrick.h"
#include "EffectCoin.h"
#include "EffectPoint.h"
#include "Box.h"
#include "Font.h"
#include "Board.h"
#include "Bush.h"
#include "Card.h"
#include "Start.h"
#include "Scene.h"
#include "MusicBrick.h"
#include "BoomerangBrother.h"
#include "Grid.h"

#define MAP_1_COURSE_CLEAR_X	2635
#define MAP_1_COURSE_CLEAR_Y	275
#define MAP_1_YGAC_X			2620
#define MAP_1_YGAC_Y			300
#define MAP_1_CARD_X			2750
#define MAP_1_CARD_Y			290

#define MAP_3_COURSE_CLEAR_X	2370
#define MAP_3_COURSE_CLEAR_Y	275
#define MAP_3_YGAC_X			2350
#define MAP_3_YGAC_Y			300
#define MAP_3_CARD_X			2480
#define MAP_3_CARD_Y			290

class CPlayScene: public CScene
{
private:
	float CamX = 0, CamY = 0;
protected: 
	bool isHavePSwitch = false;
	CMario* player;					// A play scene has to have player, right? 
	CGame* game = CGame::GetInstance();
	Font* text;
	Map* map;
	DWORD playTime = GAME_TIME_LIMIT;
	vector<LPGAMEOBJECT> objects;
	vector<Unit*> units;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);
	Grid* grid;
public: 
	int section;
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void GoldBrickDestroy(int model, float x, float y);
	void QuestionBrickDropItem(float model, float x, float y);
	void ShowEffectPoint(CGameObject* obj, float model);
	void AnnounceSceneEnd(int boxState);
	CMario * GetPlayer() { return player; } 
	void TurnIntoUnit(CGameObject* obj);	
	Grid* GetGrid() { return grid; }
	void GetObjectFromGrid();
	void UpdateGrid();
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

