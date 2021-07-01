#pragma once
#include "GameObject.h"
#include "Utils.h"
#define CELL_WIDTH		300
#define CELL_HEIGHT		230

class Unit {
	friend class Grid;
private:
	float x;
	float y;
	Grid* grid;
	LPGAMEOBJECT obj;  

	Unit* prev;
	Unit* next;
public:
	Unit(Grid* grid, LPGAMEOBJECT obj, float X, float Y);
	Unit(Grid* grid, LPGAMEOBJECT obj, int gridRow, int gridCol);

	void Move(float X, float Y);
	LPGAMEOBJECT GetObj() { return this->obj; }
};
class Grid
{
	int mapWidth;
	int mapHeight;

	int numCols;
	int numRows;
	
	vector<vector<Unit*>> cells;
public:
	Grid(int mapWidth, int mapHeight,int cellWidth, int cellHeight);
	Grid(int gridCols, int gridRows);
	~Grid();

	void Add(Unit* unit);
	void Add(Unit* unit, int gridRow, int gridCol);
	void Move(Unit* unit, float x, float y);
	void Get(float camX, float camY, vector<Unit*>& listUnits);
	void Out();
	void ClearAll();
};

