#include "PlayScence.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
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
		int frame_time = atoi(tokens[i+1].c_str());
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
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: {
		float model = (float)atof(tokens[4].c_str());
		float direction = (float)atof(tokens[5].c_str());
		obj = new CGoomba(model,direction); break;
	} 
	case OBJECT_TYPE_KOOPAS: {
		float model = (float)atof(tokens[4].c_str());
		float direction = (float)atof(tokens[5].c_str());
		obj = new CKoopas(model,direction,player); break;
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
		obj = new ColorBlock(w, h,player);
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[4].c_str());
		float b = (float)atof(tokens[5].c_str());
		float scene_id = (float)atoi(tokens[6].c_str());
		obj = new CPortal(x,y,r, b, scene_id);
		break;
	}
	case OBJECT_TYPE_PIPE: {
		float model = (float)atof(tokens[4].c_str());
		obj = new Pipe(model);
		break;
	}
	case OBJECT_TYPE_FIRE_PLANT: {
		float model = (float)atof(tokens[4].c_str());
		obj = new FirePlant(player,model);
		break;
	}
	case OBJECT_TYPE_QUESTION_BRICK: {
		float model = (float) atof(tokens[4].c_str());
		obj = new QuestionBrick(x,y,model);
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
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	// General object setup
	obj->SetPosition(x, y);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
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
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
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

	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT e = objects[i];
		if (objects[i]->CheckObjectInCamera(objects[i]))
			objects[i]->Update(dt, &coObjects);

		if (e->GetType() == QUESTIONBRICK)
		{
			QuestionBrick* qb = dynamic_cast<QuestionBrick*>(e);	
			if (qb->isUnbox)
			{
				if (qb->model == QUESTION_BRICK_MODEL_COIN) {
					EffectCoin* effectCoin = new EffectCoin(qb->x + 4, qb->y - 10);
					player->PlusCoinCollect(1);
					objects.push_back(effectCoin);		
				}
				QuestionBrickDropItem(qb->GetModel(), qb->x, qb->y);
				qb->isUnbox = false;
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
				if (!isHavePSwitch) {
					objects.push_back(new PSwitch(gb->x, gb->y - QUESTION_BRICK_BBOX_HEIGHT));
					isHavePSwitch = true;
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
							if (goldbrick->CheckObjectInCamera(goldbrick))
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
		if (e->GetType() == BOX) {
			Box* box = dynamic_cast<Box*>(e);
			if (box->isUnbox) {
				player->SetState(MARIO_STATE_WALK_RIGHT);
				player->x++;
			}
		}
	}

	for (size_t i = 0; i < objects.size(); i++) {
		if (objects.at(i)->isFinish)
			objects.erase(objects.begin() + i);
	}

	Camera* camera = new Camera(player, game, map);
	camera->Update(dt);
	 //skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

}


void CPlayScene::Render()
{
	map->DrawMap();
	for (size_t i = 0; i < objects.size(); i++) {

		objects[i]->Render();

		if (objects[i]->GetType() == BOX) {
			Box* box = dynamic_cast<Box*>(objects[i]);
			if (box->isUnbox) {
				AnnounceSceneEnd(box->stateUnbox);
			}
		}
	}
	Board* board = new Board(CGame::GetInstance()->GetCamX(), CGame::GetInstance()->GetCamY() + SCREEN_HEIGHT - DISTANCE_FROM_BOTTOM_CAM_TO_TOP_BOARD);
	board->Render(player, 999);
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
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
	case DIK_SPACE:
		mario->StartLimitJump();
		break;
	case DIK_A:
		mario->Reset();
		break;
	case DIK_F1:
		mario->y -= 20;
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_F2:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_F3:
		mario->y -= 20;
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_F4:
		mario->y -= 20;
		mario->SetLevel(MARIO_LEVEL_RACOON);
		break;
	case DIK_Q://-----------------------SHOOT FIRE--------------------------
		if (mario->level == MARIO_LEVEL_FIRE)
			mario->SetState(MARIO_STATE_SHOOT_FIRE);
		break;
	case DIK_E:
		if (game->IsKeyDown(DIK_Q)) {			
			mario->StartLimitFly();
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

	if (game->IsKeyDown(DIK_RIGHT)) {
		if (game->IsKeyDown(DIK_Q)) {
			mario->SetState(MARIO_STATE_RUN_RIGHT);
			if (game->IsKeyDown(DIK_E) && mario->state == MARIO_STATE_RUN_MAXSPEED) {
				mario->isFlying = true;
				mario->SetState(MARIO_STATE_FLY_RIGHT);
			}
		}
		else mario->SetState(MARIO_STATE_WALK_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (game->IsKeyDown(DIK_Q)) {
			mario->SetState(MARIO_STATE_RUN_LEFT);
			if (game->IsKeyDown(DIK_E) && mario->state == MARIO_STATE_RUN_MAXSPEED) {
				mario->isFlying = true;
				mario->SetState(MARIO_STATE_FLY_LEFT);
			}
		}
		else mario->SetState(MARIO_STATE_WALK_LEFT);
	}
	else mario->SetState(MARIO_STATE_IDLE);

	//-------------------------SIT------------------------
	if (game->IsKeyDown(DIK_DOWN) && !(game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT))) {
		if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_STATE_SIT);
	}
	//-------------------------JUMP------------------------
	if (game->IsKeyDown(DIK_SPACE)) {
		if (mario->vy <= 0) {
			mario->SetState(MARIO_STATE_JUMP);
		}
		else if (mario->level == MARIO_LEVEL_RACOON) {
			mario->SetState(MARIO_STATE_JUMP);
		}
			
	}

	//-----------------------SPIN-------------------------
	if (mario->level == MARIO_LEVEL_RACOON && game->IsKeyDown(DIK_Q)) {
		if (!mario->isHolding)
			if (!(game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_SPACE)))
				mario->SetState(MARIO_STATE_SPIN);
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) {
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
 
	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->y -= 10;
		mario->SetState(MARIO_STATE_IDLE);
		break;
	case DIK_Q:
		mario->isHolding = false;
		mario->SetState(MARIO_STATE_KICK);
	}
}


void CPlayScene::GoldBrickDestroy(int model, float x, float y)
{
	switch (model)
	{
	case GOLD_BRICK_MODEL_PSWITCH:
	{
			objects.push_back(new PSwitch(x, y));
		break;
	}
	//case GB_CONTAIN_MUSHROOM_1_UP:
	//{
	//	ListItem.push_back(new CMushRoom(x, y + 10, MUSHROOM_GREEN));
	//	break;
	//}
	//case GB_CONTAIN_POWER_UP:
	//{
	//	if (player->level == MARIO_LEVEL_SMALL)
	//	{
	//		ListItem.push_back(new CMushRoom(x, y + 10, MUSHROOM_RED));
	//	}
	//	if (player->level == MARIO_LEVEL_BIG)
	//	{
	//		ListItem.push_back(new Leaf(x, y));
	//	}
	//	if (player->level == MARIO_LEVEL_RACCOON || player->level == MARIO_LEVEL_FIRE)
	//	{
	//		ListItem.push_back(new Flower(x, y));
	//	}
	//	break;
	//}
	}
}

void CPlayScene::QuestionBrickDropItem(float model, float x, float y) {
	
	switch ((int)model) {
	case QUESTION_BRICK_MODEL_COIN:
		break;
	case QUESTION_BRICK_MODEL_POWER_UP:
		if (player->GetLevel() == MARIO_LEVEL_SMALL)
			objects.push_back(new Mushroom(x, y - 10, MUSHROOM_MODEL_RED));
		if (player->GetLevel() == MARIO_LEVEL_BIG || player->GetLevel() == MARIO_LEVEL_RACOON)
			objects.push_back(new Leaf(x, y - 10));
	}
}

void CPlayScene::ShowEffectPoint(CGameObject* obj, float model) {
	EffectPoint* effectPoint = new EffectPoint(obj->x, obj->y, model);
	objects.push_back(effectPoint);
}

void CPlayScene::AnnounceSceneEnd(int boxState) {
	text = new Font();
	text->Draw(2635, 275, "COURSE CLEAR !");
	text->Draw(2620, 300, "YOU GOT A CARD");
	LPDIRECT3DTEXTURE9 Tex = CTextures::GetInstance()->Get(TEXID_FONT35);
	if (boxState == BOX_STATE_MUSHROOM) {
		LPSPRITE SpriteTile = new CSprite(64, 187, 33, 211, 61, Tex);
		SpriteTile->Draw(2740, 290);
	}
	else if (boxState == BOX_STATE_FLOWER) {
		LPSPRITE SpriteTile = new CSprite(65, 211, 33, 235, 61, Tex);
		SpriteTile->Draw(2740, 290);
	}
	else if (boxState == BOX_STATE_STAR) {
		LPSPRITE SpriteTile = new CSprite(66, 235, 33, 259, 61, Tex);
		SpriteTile->Draw(2740, 290);
	}
}
