#include "Grid.h"

Unit::Unit(Grid* grid, LPGAMEOBJECT obj, float X, float Y) {
	this->grid = grid;
	this->obj = obj;
	this->x = X;
	this->y = Y;

	this->prev = NULL;
	this->next = NULL;

	grid->Add(this);
}

Unit::Unit(Grid * grid, LPGAMEOBJECT obj, int gridRow, int gridCol) {
	this->grid = grid;
	this->obj = obj;
	this->x = obj->x;
	this->y = obj->y;

	this->prev = NULL;
	this->next = NULL;

	grid->Add(this, gridRow, gridCol);
}

void Unit::Move(float X, float Y) {
	grid->Move(this, X, Y);
}
//-----------------------------------------------------------
Grid::Grid(int mapWidth, int mapHeight, int cellWidth, int cellHeight) {
	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;
	DebugOut(L"mapWidth %d mapHeight %d \n", mapWidth, mapHeight);
	numCols = mapWidth / cellWidth;
	numRows = mapHeight / cellHeight;

	cells.resize(numRows);

	for (int i = 0; i < numRows; i++)
		cells[i].resize(numCols);

	for (int i = 0; i < numRows; i++)
		for (int j = 0; j < numCols; j++)
			cells[i][j] = NULL;
}
void Grid::Add(Unit* unit) {
	int row = (int)(unit->y / CELL_HEIGHT);
	int col = (int)(unit->x / CELL_WIDTH);

	if (row == this->numRows)	row--;
	if (col == this->numCols)	col--;

	//add Head
	unit->prev = NULL;
	unit->next = cells[row][col];
	cells[row][col] = unit;
	if (unit->next != NULL)
		unit->next->prev = unit;
}

void Grid::Add(Unit* unit,int gridRow, int gridCol) {

	if (gridRow == this->numRows)	gridRow--;
	if (gridCol == this->numCols)	gridCol--;

	//add Head
	unit->prev = NULL;
	unit->next = cells[gridRow][gridCol];
	cells[gridRow][gridCol] = unit;
	if (unit->next != NULL)
		unit->next->prev = unit;
}

void Grid::Move(Unit* unit, float X, float Y) {
	int oldRow = (int)(unit->y / CELL_HEIGHT);
	int oldCol = (int)(unit->x / CELL_WIDTH);

	int newRow = (int)(Y / CELL_HEIGHT);
	int newCol = (int)(X / CELL_WIDTH);

	//if obj is outside game then no need update
	if (newRow < 0 || newRow >= numRows || newCol < 0 || newCol >= numCols) return;

	unit->x = X;
	unit->y = Y;
	if (oldRow == newRow && oldCol == newCol) return;//if obj stay the same then no need update

	if (unit->prev != NULL)
		unit->prev->next = unit->next;
	if (unit->next != NULL)
		unit->next->prev = unit->prev;
	if (cells[oldRow][oldCol] == unit)
		cells[oldRow][oldCol] = unit->next;

	Add(unit);
}

void Grid::Get(float camX, float camY, vector<Unit*>&listUnits) {
	int startCol = (int)((camX) / CELL_WIDTH);
	int endCol = (int)ceil((camX + SCREEN_WIDTH) / CELL_WIDTH);
	int ENDCOL = (int)ceil(mapWidth / CELL_WIDTH);
	if (endCol > ENDCOL)	endCol = ENDCOL;
	if (startCol < 0) startCol = 0;

	int startRow = (int)(camY / CELL_HEIGHT);
	int endRow = (int)ceil((camY + SCREEN_HEIGHT) / CELL_HEIGHT);
	int ENDROW = (int)ceil(mapHeight / CELL_HEIGHT);
	if (endRow > ENDROW) endRow = ENDROW;
	if (startRow < 0) startRow = 0;

	for (size_t i = startRow; i < endRow; i++)
	{
		for (size_t j = startCol; j < endCol; j++)
		{
			Unit* unit = cells[i][j];
			while (unit != NULL) {
				if (!unit->GetObj()->isFinish) {
					listUnits.push_back(unit);
					unit = unit->next;				
				}
				else {
					if (cells[i][j] == unit)
						cells[i][j] = unit->next;
					if (unit->next != NULL)
						unit->next->prev = unit->prev;
					if (unit->prev != NULL)
						unit->prev->next = unit->next;

					Unit* temp = unit;
					unit = unit->next;
					if(temp->GetObj() != NULL)
						delete temp->GetObj();
					delete temp;
				}
			}
		}
	}
}
void Grid::Out() {
	int count = 0;
	for (size_t i = 0; i < numRows; i++)
	{
		for (size_t j = 0; j < numCols; j++)
		{
			Unit* unit = cells[i][j];
			while (unit) {
				count++;
				unit = unit->next;
			}
		}
	}
}

void Grid::ClearAll() {
	for (size_t i = 0; i < numRows; i++)
	{
		for (size_t j = 0; j < numCols; j++)
		{
			Unit* unit = cells[i][j];
			while (unit != NULL) {
				if (cells[i][j] == unit)
					cells[i][j] = unit->next;
				if (unit->next != NULL)
					unit->next->prev = unit->prev;
				if (unit->prev != NULL)
					unit->prev->next = unit->next;

				Unit* temp = unit;
				unit = unit->next;
				delete temp->GetObj();
				delete temp;
			}
		}
	}
}

Grid::Grid(int gridCols, int gridRows) {

	this->numCols = gridCols;
	this->numRows = gridRows;

	this->mapWidth = gridCols * CELL_WIDTH;
	this->mapHeight = gridRows * CELL_HEIGHT;

	cells.resize(numRows);

	for (size_t i = 0; i < numRows; i++)
	{
		cells[i].resize(numCols);
	}
	for (size_t i = 0; i < numRows; i++)
	{
		for (size_t j = 0; j < numCols; j++)
		{
			cells[i][j] = NULL;
		}
	}
}

Grid::~Grid()
{
}
