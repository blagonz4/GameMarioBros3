#include "PlayScence.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (size_t i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}
	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (size_t i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f; f.open(path);
	if (!f)
		DebugOut(L"\nFailed to open object file!");

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE)) {

		string line(str);
		vector<string> tokens = split(line);

		DebugOut(L"--> %s\n", ToWSTR(line).c_str());

		if (line[0] == '#') continue;
		//if (tokens.size() < 3) continue; // skip invalid lines - an object set must have at least id, x, y

		int object_type = atoi(tokens[0].c_str());
		float x = (float)atof(tokens[1].c_str());
		float y = (float)atof(tokens[2].c_str());
		int ani_set_id = atoi(tokens[3].c_str());

		CAnimationSets * animation_sets = CAnimationSets::GetInstance();

		CGameObject *obj = NULL;

		switch (object_type)
		{
		case OBJECT_TYPE_MARIO:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new CMario(x, y);
			player = (CMario*)obj;

			DebugOut(L"[INFO] Player object created!\n");
			break;
		//case GRID: {
		//	//DebugOut(L"object: %d \n", atoi(tokens[0].c_str()));
		//	int gridRows = atoi(tokens[1].c_str());
		//	int gridCols = atoi(tokens[2].c_str());
		//	grid = new Grid(gridCols, gridRows);
		//	break;
		//}
		case OBJECT_TYPE_GOOMBA: {
			float model = (float)atof(tokens[4].c_str());
			float direction = (float)atof(tokens[5].c_str());
			obj = new CGoomba(player, model, direction); break;
		}
		case OBJECT_TYPE_KOOPAS: {
			float model = (float)atof(tokens[4].c_str());
			float direction = (float)atof(tokens[5].c_str());
			obj = new CKoopas(model, direction, player); break;
		}
		case OBJECT_TYPE_BRICK: {
			obj = new CBrick();
			break;
		}
		case OBJECT_TYPE_PLATFORM: {
			float w = (float)atof(tokens[4].c_str());
			float h = (float)atof(tokens[5].c_str());
			obj = new Platform(w, h);
			break;
		}
		case OBJECT_TYPE_COLOR_BLOCK: {
			float w = (float)atof(tokens[4].c_str());
			float h = (float)atof(tokens[5].c_str());
			obj = new ColorBlock(w, h, player);
			break;
		}
		case OBJECT_TYPE_PORTAL:
		{
			float r = (float)atof(tokens[4].c_str());
			float b = (float)atof(tokens[5].c_str());
			float scene_id = (float)atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
			break;
		}
		case OBJECT_TYPE_PIPE: {
			float model = (float)atof(tokens[4].c_str());
			obj = new Pipe(x, y, model);
			break;
		}
		case OBJECT_TYPE_FIRE_PLANT: {
			float model = (float)atof(tokens[4].c_str());
			obj = new FirePlant(x, y, player, model);
			break;
		}
		case OBJECT_TYPE_QUESTION_BRICK: {
			float model = (float)atof(tokens[4].c_str());
			obj = new QuestionBrick(x, y, model);
			break;
		}
		case OBJECT_TYPE_GOLD_BRICK: {
			float model = (float)atof(tokens[4].c_str());
			obj = new GoldBrick(x, y, model);
			break;
		}
		case OBJECT_TYPE_COIN: {
			obj = new Coin();
			break;
		}
		case OBJECT_TYPE_BOX:
		{
			obj = new Box();
			break;
		}
		case OBJECT_TYPE_BUSH:
			obj = new Bush();
			break;
		case OBJECT_TYPE_MUSIC_BRICK: {
			float model = (float)atof(tokens[4].c_str());
			obj = new MusicBrick(x, y, model);
			break;
		}
		case OBJECT_TYPE_BOOMERANG_BROTHER: {
			float dir = (float)atof(tokens[4].c_str());
			obj = new BoomerangBrother(x,y, dir);
			break;
		}
		case OBJECT_TYPE_MUSHROOM: {
			float model = (float)atof(tokens[4].c_str());
			obj = new Mushroom(x, y, model);
			break;
		}
		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
		}
		//if (object_type != GRID) {// General object setup
		//	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		//	obj->SetPosition(x, y);
		//	obj->SetAnimationSet(ani_set);

		//	if (object_type == OBJECT_TYPE_MARIO || object_type == OBJECT_TYPE_PLATFORM)
		//		objects.push_back(obj);

		//}
		//if (object_type != OBJECT_TYPE_MARIO && object_type != GRID && object_type != OBJECT_TYPE_PLATFORM) {
		//	int gridCol = (int)atoi(tokens[tokens.size() - 1].c_str());
		//	int gridRow = (int)atoi(tokens[tokens.size() - 2].c_str());
		//	Unit* unit = new Unit(grid, obj, gridRow, gridCol);
		//}

		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);

	}

	f.close();
	////grid->Out();
}

void CPlayScene::_ParseSection_TILEMAP(string line) {	//doc map tu file txt

	int ID, rowMap, columnMap, rowTile, columnTile, totalTile;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f(path, ios::in);
	f >> ID >> rowMap >> columnMap >> rowTile >> columnTile >> totalTile;
	int** tileMapData = new int *[rowMap];
	for (int i = 0; i < rowMap; i++) {
		tileMapData[i] = new int[columnMap];
		for (int j = 0; j < columnMap; j++) {
			f >> tileMapData[i][j];
		}
	}
	f.close();
	map = new Map(ID, rowMap, columnMap, rowTile, columnTile, totalTile);
	map->GetSpriteTile();
	map->SetMap(tileMapData);
}



void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TILEMAP]") {
			section = SCENE_SECTION_DRAWMAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_DRAWMAP: _ParseSection_TILEMAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	CGameObject* obj = NULL;

	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();
	//GetObjectFromGrid();

	playTime -= dt;

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT e = objects[i];

		if (objects[i]->CheckObjectInCamera())
			objects[i]->Update(dt, &coObjects);
		//else objects[i]->Update(0, &coObjects);


		if (isEndScene_1 && player != NULL) {
			player->SetState(MARIO_STATE_WALK_RIGHT);
			player->vx = 0.1f;
		}

		if (e->GetType() == QUESTIONBRICK)
		{
			QuestionBrick* qb = dynamic_cast<QuestionBrick*>(e);
			if (qb->isUnbox)
			{
				if (qb->model == QUESTION_BRICK_MODEL_COIN) {
					EffectCoin* effectCoin = new EffectCoin(qb->x, qb->y - 10);
					player->PlusCoinCollect(1);
					objects.push_back(effectCoin);		
				}
				QuestionBrickDropItem(qb->GetModel(), qb->x, qb->y);
				qb->isUnbox = false;
			}
		}
		if (e->GetType() == BRICK)
		{
			CBrick* brick = dynamic_cast<CBrick*>(e);
			if (brick->isUnbox == 1)
			{
				brick->isUnbox = 2;
				Leaf* leaf = new Leaf(brick->x, brick->y - 10);
				//TurnIntoUnit(leaf);
				objects.push_back(leaf);
				return;
			}
		}
		if (e->GetType() == GOLDBRICK)
		{
			GoldBrick* gb = dynamic_cast<GoldBrick*>(e);
			if (gb->isFinish) {
				objects.push_back(new EffectBrokenBrick(gb->x, gb->y, 2));	//he so nx de hieu ung bay cho dep 
				objects.push_back(new EffectBrokenBrick(gb->x, gb->y, 5));
				objects.push_back(new EffectBrokenBrick(gb->x, gb->y, -2));
				objects.push_back(new EffectBrokenBrick(gb->x, gb->y, -5));
			}
			if (gb->state == GOLD_BRICK_STATE_UNBOX)
			{
				if (!isHavePSwitch && gb->model == GOLD_BRICK_MODEL_PSWITCH) {
					PSwitch* pswitch = new PSwitch(gb->x, gb->y - QUESTION_BRICK_BBOX_HEIGHT);
					objects.push_back(pswitch);
					isHavePSwitch = true;
				}
				if (!gb->isUnbox && gb->model == GOLD_BRICK_MODEL_MUSHROOM_1_UP) {
					QuestionBrickDropItem(gb->model, gb->x, gb->y);
					gb->isUnbox = true;
				}
			}
		}
		if (e->GetType() == PSWITCH)
		{
			PSwitch* pswitch = dynamic_cast<PSwitch*>(e);
			if (pswitch->GetState() == PSWITCH_STATE_USED && !pswitch->isUsed)
			{
				for (UINT i = 0; i < objects.size(); i++)
				{
					if (objects[i]->GetType() == GOLDBRICK)
					{
						GoldBrick* goldbrick = dynamic_cast<GoldBrick*>(objects[i]);
						if (goldbrick->model == GOLD_BRICK_MODEL_COIN)
						{
							if (goldbrick->CheckObjectInCamera())
							{
								goldbrick->SetState(GOLD_BRICK_STATE_IDLE_COIN);
							}
						}
					}
				}
				pswitch->isUsed = true;
			}
		}
		if (e->GetType() == EFFECT_COIN) {
			EffectCoin* effectCoin = dynamic_cast<EffectCoin*>(e);
			if (effectCoin->isFinish) {
				EffectPoint* effectPoint = new EffectPoint(effectCoin->x,
					effectCoin->y,
					POINT_EFFECT_MODEL_100);
				//TurnIntoUnit(effectPoint);
				player->PlusScore(100);
				objects.push_back(effectPoint);
			}
		}
		if (e->GetType() == GOOMBA) {
			CGoomba* goomba = dynamic_cast<CGoomba*>(e);
			if (goomba->isFinish)
				if (goomba->GetModel() == GOOMBA_MODEL_NORMAL) {
					ShowEffectPoint(goomba, POINT_EFFECT_MODEL_100);
					player->PlusScore(100);
				}
				else {
					player->PlusScore(200);
					ShowEffectPoint(goomba, POINT_EFFECT_MODEL_200);
				}
		}
		if (e->GetType() == KOOPAS) {
			CKoopas* koopa = dynamic_cast<CKoopas*>(e);
			if (koopa->isFinish) {
				player->PlusScore(100);
				ShowEffectPoint(koopa, POINT_EFFECT_MODEL_100);
			}
		}
		if (e->GetType() == BOOMERANGBROTHER) {
			BoomerangBrother* bb = dynamic_cast<BoomerangBrother*>(e);
			if (bb->isFinish) {
				EffectTailHit* effectTailHit = new EffectTailHit(bb->x, bb->y);
				objects.push_back(effectTailHit);
				player->PlusScore(200);
				ShowEffectPoint(bb, POINT_EFFECT_MODEL_200);
			}
		}
		if (e->GetType() == FIREPLANT) {
			FirePlant* plant = dynamic_cast<FirePlant*>(e);
			if (plant->isFinish) {
				EffectTailHit* effectTailHit = new EffectTailHit(plant->x, plant->y);
				objects.push_back(effectTailHit);
				player->PlusScore(200);
				ShowEffectPoint(plant, POINT_EFFECT_MODEL_200);
			}
		}
	}

	for (size_t i = 0; i < objects.size(); i++) {
		if (objects.at(i)->isFinish)
			objects.erase(objects.begin() + i);
	}

	Camera* camera = new Camera(player, game, map);
	camera->Update(dt);
	//UpdateGrid();

	//skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;
}

void CPlayScene::GetObjectFromGrid() {
	units.clear();
	for (size_t i = 0; i < objects.size(); i++) {
		LPGAMEOBJECT e = objects[i];
		if (e->GetType() != PLATFORM && e->GetType() != MARIO)
			objects.erase(objects.begin() + i);
	}
	//objects.clear();

	CGame* game = CGame::GetInstance();
	float camX, camY;

	camX = game->GetCamX();
	camY = game->GetCamY();

	grid->Get(camX, camY, units);

	for (UINT i = 0; i < units.size(); i++) {
		LPGAMEOBJECT obj = units[i]->GetObj();
		objects.push_back(obj);
	}
}
void CPlayScene::UpdateGrid() {
	for (size_t i = 0; i < units.size(); i++) {
		LPGAMEOBJECT obj = units[i]->GetObj();
		float newPosX, newPosY;
		obj->GetPosition(newPosX, newPosY);
		units[i]->Move(newPosX, newPosY);
	}
}

void CPlayScene::Render()
{
	map->DrawMap();
	for (size_t i = 0; i < objects.size(); i++) {

		if (objects[i]->CheckObjectInCamera())
			objects[i]->Render();

		if (objects[i]->GetType() == BOX) {
			Box* box = dynamic_cast<Box*>(objects[i]);
			if (box->isUnbox) {
				AnnounceSceneEnd(box->stateUnbox);
			}
		}
	}
	Board* board = new Board(CGame::GetInstance()->GetCamX(), CGame::GetInstance()->GetCamY() + SCREEN_HEIGHT - DISTANCE_FROM_BOTTOM_CAM_TO_TOP_BOARD);
	board->Render(player, playTime);
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	//for (size_t i = 0; i < objects.size(); i++)
	//	delete objects[i];
	isEndScene_1 = false;
	game->SetCamPos(0, 100); //Set cam tu map 1_3 qua map phu 
	objects.clear();
	if (grid != NULL)
		grid->ClearAll();
	objects.clear();
	//units.clear();
	player = NULL;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

		switch (KeyCode)
		{
		case DIK_S:
			if (mario->isOnGround) {
				mario->isReadyToJump = true;
			}
			else if (mario->level == MARIO_LEVEL_RACOON && (!mario->isFlying && !mario->isFlapping && mario->vy > 0)) {
				mario->StartFlapping();
			}
			if (mario->isFlying && mario->level == MARIO_LEVEL_RACOON)
				mario->StartTailFlying();
			break;
		case DIK_R:
			mario->Reset();
			break;
		case DIK_1:
			mario->SetLevel(MARIO_LEVEL_SMALL);
			break;
		case DIK_2:
			mario->y -= 20;
			mario->SetLevel(MARIO_LEVEL_BIG);
			break;
		case DIK_3:
			mario->y -= 20;
			mario->SetLevel(MARIO_LEVEL_RACOON);
			break;
		case DIK_4:
			mario->y -= 20;
			mario->SetLevel(MARIO_LEVEL_FIRE);
			break;
		case DIK_5:
			if (game->GetInstance()->GetScene() == MAP1_1)
				mario->SetPosition(480, 300);
			if (game->GetInstance()->GetScene() == MAP1_3)
				mario->SetPosition(530, 300);
			break;
		case DIK_6:
			if (game->GetInstance()->GetScene() == MAP1_1)
				mario->SetPosition(2020, 300);
			if (game->GetInstance()->GetScene() == MAP1_3)
				mario->SetPosition(1088, 300);
			break;
		case DIK_7:
			if (game->GetInstance()->GetScene() == MAP1_1)
				mario->SetPosition(1414, 50);
			if (game->GetInstance()->GetScene() == MAP1_3)
				mario->SetPosition(500, 10);
			break;
		case DIK_8:
			if (game->GetInstance()->GetScene() == MAP1_1)
			mario->SetPosition(2256, 30);
			if (game->GetInstance()->GetScene() == MAP1_3)
				mario->SetPosition(1600, 300);
			break;
		case DIK_9:
			if (game->GetInstance()->GetScene() == MAP1_1)
				mario->SetPosition(2500, 300);
			if (game->GetInstance()->GetScene() == MAP1_3)
				mario->SetPosition(2300, 300);
			break;
		case DIK_DOWN:
			mario->isWannaDown = true;
			break;
		case DIK_A://-----------------------SHOOT FIRE--------------------------
			if (mario->level == MARIO_LEVEL_FIRE && !mario->isShooting && !mario->isSitting) {
				mario->SetState(MARIO_STATE_SHOOT_FIRE);
				mario->StartShooting();
			}
			if (mario->level == MARIO_LEVEL_RACOON && !mario->isTurningTail && !mario->isSitting) {
					mario->StartTurning();
			}
			break;
		}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;

	//--------------------RUN/TURN/FLY/WALK----------------------------
	if (game->IsKeyDown(DIK_A)) {
		mario->isReadyToHold = true;
		if (!mario->isRunning && mario->vx != 0 && mario->isReadyToRun) {
			if(mario->nx > 0)	mario->SetState(MARIO_STATE_RUN_RIGHT);
			else mario->SetState(MARIO_STATE_RUN_LEFT);
			mario->StartRunning();
		}
			
	}
	if (game->IsKeyDown(DIK_S) && mario->isReadyToJump) {
		mario->SetState(MARIO_STATE_JUMP);
		mario->isJumping = true;
		mario->isReadyToSit = false;
	}
	else if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALK_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALK_LEFT);
	else if (game->IsKeyDown(DIK_DOWN) && mario->isReadyToSit && mario->level != MARIO_LEVEL_SMALL)
		mario->SetState(MARIO_STATE_SIT);
	else mario->SetState(MARIO_STATE_IDLE);
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) {
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode){
	case DIK_S:
		if (!mario->isOnGround) {
			mario->isReadyToJump = false;
			mario->isFlapping = false;
		}
		break;
	case DIK_DOWN:
		mario->isSitting = false;
		mario->isWannaDown = false;
		break;
	case DIK_A:
		mario->isHolding = false;
		mario->isReadyToHold = false;
		mario->StopRunning();
		//mario->isHolding ? mario->isHolding = false : mario->isHolding = true;
		break;
	}
}


void CPlayScene::QuestionBrickDropItem(float model, float x, float y) {

	switch ((int)model) {
	case QUESTION_BRICK_MODEL_COIN:
		break;
	case QUESTION_BRICK_MODEL_POWER_UP:
	case GOLD_BRICK_MODEL_MUSHROOM_1_UP:
		if (player->GetLevel() == MARIO_LEVEL_SMALL) {
			Mushroom* mr = new Mushroom(x, y, MUSHROOM_MODEL_RED);
			objects.push_back(mr);
			//TurnIntoUnit(mr);
		}
		if (player->GetLevel() == MARIO_LEVEL_BIG || player->GetLevel() == MARIO_LEVEL_RACOON) {
			Leaf* leaf = new Leaf(x, y - 10);
			objects.push_back(leaf);
			//TurnIntoUnit(leaf);
		}
	}
}

void CPlayScene::ShowEffectPoint(CGameObject* obj, float model) {
	EffectPoint* effectPoint = new EffectPoint(obj->x, obj->y, model);
	//TurnIntoUnit(effectPoint);
	objects.push_back(effectPoint);
}

void CPlayScene::AnnounceSceneEnd(int boxState) {
	text = new Font();
	if (game->GetScene() == MAP1_1) {
		text->Draw(MAP_1_COURSE_CLEAR_X, MAP_1_COURSE_CLEAR_Y, "COURSE CLEAR !");
		text->Draw(MAP_1_YGAC_X, MAP_1_YGAC_Y, "YOU GOT A CARD");
		LPDIRECT3DTEXTURE9 Tex = CTextures::GetInstance()->Get(TEXID_FONT35);
		if (boxState == BOX_STATE_MUSHROOM) {
			LPSPRITE SpriteTile = new CSprite(64, 187, 33, 211, 61, Tex);
			SpriteTile->Draw(MAP_1_CARD_X, MAP_1_CARD_Y);
		}
		else if (boxState == BOX_STATE_FLOWER) {
			LPSPRITE SpriteTile = new CSprite(65, 211, 33, 235, 61, Tex);
			SpriteTile->Draw(MAP_1_CARD_X, MAP_1_CARD_Y);
		}
		else if (boxState == BOX_STATE_STAR) {
			LPSPRITE SpriteTile = new CSprite(66, 235, 33, 259, 61, Tex);
			SpriteTile->Draw(MAP_1_CARD_X, MAP_1_CARD_Y);
		}
	}
	else // map
	{
		text->Draw(MAP_3_COURSE_CLEAR_X, MAP_1_COURSE_CLEAR_Y, "COURSE CLEAR !");
		text->Draw(MAP_3_YGAC_X, MAP_1_YGAC_Y, "YOU GOT A CARD");
		LPDIRECT3DTEXTURE9 Tex = CTextures::GetInstance()->Get(TEXID_FONT35);
		if (boxState == BOX_STATE_MUSHROOM) {
			LPSPRITE SpriteTile = new CSprite(64, 187, 33, 211, 61, Tex);
			SpriteTile->Draw(MAP_3_CARD_X, MAP_1_CARD_Y);
		}
		else if (boxState == BOX_STATE_FLOWER) {
			LPSPRITE SpriteTile = new CSprite(65, 211, 33, 235, 61, Tex);
			SpriteTile->Draw(MAP_3_CARD_X, MAP_1_CARD_Y);
		}
		else if (boxState == BOX_STATE_STAR) {
			LPSPRITE SpriteTile = new CSprite(66, 235, 33, 259, 61, Tex);
			SpriteTile->Draw(MAP_3_CARD_X, MAP_1_CARD_Y);
		}
	}
	
}

void CPlayScene::TurnIntoUnit(CGameObject* obj) {
	//Unit* unit = new Unit(grid, obj, obj->x, obj->y);
}

void CPlayScene::LoadBackUp() {
	BackUp* backup = BackUp::GetInstance();
	backup->LoadBackUp(player);
}

void CPlayScene::BackUpPlayer() {
	if (player != NULL) {
		BackUp* backup = BackUp::GetInstance();
		backup->BackUpMario(player);
	}
}