#pragma once
#include "Game.h"
#include "Sprites.h"
#include <fstream>
#include "Utils.h"
#include <string>
#include "Textures.h"
#include "define.h"

class Map {
	
	static Map* __instance;
	LPDIRECT3DTEXTURE9 Tex;
	int ColumnMap, RowMap;
	int ColumnTile, RowTile;
	int TotalTile;
	int** TileMap;
	vector<LPSPRITE> Tiles;
	float CamX, CamY;

public:
	Map(int TexID,
		int RowMap,
		int ColumnMap,
		int RowTile,
		int ColumnTile,
		int TotalTile);
	~Map();
	void SetMap(int ** tilemap);
	void GetSpriteTile();
	void DrawMap();
	void SetCamPos(float x, float y) {
		CamX = x;
		CamY = y;
	}
	float GetMapWidth();
	float GetMapHeight();

};
