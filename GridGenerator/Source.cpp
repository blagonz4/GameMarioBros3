#include "conio.h"
#include "stdio.h"
#include "string.h"
#include "iostream"
#include <fstream>
#include <vector>
#include "../05-ScenceManager/Utils.cpp"


using namespace std;

#define MAX_SCENE_LINE			1024
#define TILESET_HEIGHT			16
#define TILESET_WIDTH			16
#define CELL_WIDTH				300
#define CELL_HEIGHT				230
#define GRID					999

void CreateGridFile(int sceneId);
void ReadMapInformation(int sceneId, int& mapWidth, int& mapHeight);
void WriteGridFile(int sceneId, int gridCols, int gridRows);

int main() {
	int sceneId;
	cout << "Insert Scene ID from 1 to 4: ";
	cin >> sceneId;
	CreateGridFile(sceneId);
	cout << "\n";
	system("Pause");
	return 0;
}

void CreateGridFile(int sceneId) {
	int mapWidth = -1, mapHeight = -1;
	ReadMapInformation(sceneId,mapHeight,mapWidth);

	if (mapWidth == -1 || mapHeight == -1) return;

	int gridCols = ceil((float)mapWidth / CELL_WIDTH);
	int gridRows = ceil((float)mapHeight / CELL_HEIGHT);

	cout << "\n Columns of Grid: \t" << gridCols << " \n Rows of Grid: \t" << gridRows;

	WriteGridFile(sceneId, gridCols, gridRows);
}

void ReadMapInformation(int sceneId, int& mapHeight, int& mapWidth) {
	char fileMapName[200];
	
	if (sceneId == 1 || sceneId == 3) {
		sprintf_s(fileMapName, "..\\05-ScenceManager\\Tilemap\\Map1_%d\\map1_%d_map.txt", sceneId,sceneId);
	}		
	else if (sceneId == 2) {
		sprintf_s(fileMapName, "..\\05-ScenceManager\\Tilemap\\Map1_1_1\\map1_1_1_map.txt");
	}
	else if (sceneId == 4) {
		sprintf_s(fileMapName, "..\\05-ScenceManager\\Tilemap\\Map1_3_1\\map1_3_1_map.txt");
	}
	else sprintf_s(fileMapName, "..\\05-ScenceManager\\Tilemap\\Worldmap\\worldmap_map.txt");

	ifstream mapFile(fileMapName);
	if (!mapFile) {
		cout << "Could not open the map file!";
	}
	else {
		cout << "Open map successfully" << endl;
		char str[MAX_SCENE_LINE];
		mapFile.getline(str, MAX_SCENE_LINE);
		string line(str);

		vector<string> tokens = split(line);
		if (tokens.size() < 3) return;

		int totalRowMap = atoi(tokens[1].c_str());
		int totalColMap = atoi(tokens[2].c_str());

		mapWidth = totalColMap * TILESET_WIDTH;
		mapHeight = totalRowMap * TILESET_HEIGHT;

		cout << "\n totalRowMap: \t" << totalRowMap << "\n totalColMap: \t" << totalColMap;
		cout << "\n mapWidth: \t" << mapWidth << "\n mapHeight: \t" << mapHeight;
	}
	mapFile.close();
}

void WriteGridFile(int sceneId, int gridCols, int gridRows) {
	char gridFileName[100];
	int mapId = sceneId;

	if (sceneId == 1 || sceneId == 3) {
		sprintf_s(gridFileName, "..\\05-ScenceManager\\Tilemap\\Map1_%d\\map1_%d_grid.txt", sceneId, sceneId);
	}
	else if (sceneId == 2) {
		sprintf_s(gridFileName, "..\\05-ScenceManager\\Tilemap\\Map1_1_1\\map1_1_1_grid.txt");
	}
	else if (sceneId == 4) {
		sprintf_s(gridFileName, "..\\05-ScenceManager\\Tilemap\\Map1_3_1\\map1_3_1_grid.txt");
	}
	else sprintf_s(gridFileName, "..\\05-ScenceManager\\Tilemap\\Worldmap\\worldmap_grid.txt");

	ofstream gridFile;
	gridFile.open(gridFileName);
	gridFile << GRID << "\t" << gridRows << "\t" << gridCols << "\t" << 0;

	char objectFileName[100];
	if (sceneId == 1 || sceneId == 3) {
		sprintf_s(objectFileName, "..\\05-ScenceManager\\Tilemap\\Map1_%d\\map1_%d_object.txt", sceneId, sceneId);
	}
	else if (sceneId == 2) {
		sprintf_s(objectFileName, "..\\05-ScenceManager\\Tilemap\\Map1_1_1\\map1_1_1_object.txt");
	}
	else if (sceneId == 4) {
		sprintf_s(objectFileName, "..\\05-ScenceManager\\Tilemap\\Map1_3_1\\map1_3_1_object.txt");
	}
	else sprintf_s(objectFileName, "..\\05-ScenceManager\\Tilemap\\Worldmap\\worldmap_object.txt");

	ifstream objectFile(objectFileName);
	char str[1024];

	while (objectFile.getline(str, 1024)) {
		string line(str);
		vector<string> tokens = split(line);

		if (line[0] != '#' && //#
			line[0] != '0') {//MARIO
			if (tokens.size() < 3)	continue;

			float x = (float)atof(tokens[1].c_str());
			float y = (float)atof(tokens[2].c_str());

			int col = (int)(x / CELL_WIDTH);
			int row = (int)(y / CELL_HEIGHT);
			
			if (row == gridRows)	row--;
			if (col == gridCols)	col--;

			gridFile << "\n" << line << "\t" << row << "\t" << col;
		}
		else {
			gridFile << "\n" << line;
		}
		
		cout << "\n" << line;
	}
	objectFile.close();
	gridFile.close();
}