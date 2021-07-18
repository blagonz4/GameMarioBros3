#pragma once
#include <iostream>
#include <fstream>
#include "Game.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "define.h"
#include "Platform.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Map.h"
#include "Mario.h"
#include "Camera.h"
#include "Box.h"
#include "Font.h"
#include "Board.h"
#include "Bush.h"
#include "Card.h"
#include "KeyEventHandler.h"
#include "Start.h"
#include "Scene.h"
#include "Grid.h"
#include "BackUp.h"
class WorldMap : public CScene
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
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);

public:
	int section;
	WorldMap(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void BackUpPlayer();
	void LoadBackUp();
	CMario * GetPlayer() { return player; }

};

class WorldMapKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

	WorldMapKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

