#include "IntroScene.h"

using namespace std;

IntroScene::IntroScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new IntroSceneKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

void IntroScene::_ParseSection_TEXTURES(string line)
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

void IntroScene::_ParseSection_SPRITES(string line)
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

void IntroScene::_ParseSection_ANIMATIONS(string line)
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

void IntroScene::_ParseSection_ANIMATION_SETS(string line)
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
void IntroScene::_ParseSection_OBJECTS(string line)
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
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: {
		float model = (float)atof(tokens[4].c_str());
		float direction = (float)atof(tokens[5].c_str());
		obj = new CGoomba(model, direction); break;
	}
	case OBJECT_TYPE_KOOPAS: {
		float model = (float)atof(tokens[4].c_str());
		float direction = (float)atof(tokens[5].c_str());
		obj = new CKoopas(model, direction, player); break;
	}
	case OBJECT_TYPE_PLATFORM: {
		float w = (float)atof(tokens[3].c_str());
		float h = (float)atof(tokens[4].c_str());
		obj = new Platform();
		break;
	}
	case OBJECT_TYPE_INTRO_GROUND: {
		obj = new Ground(x,y);
		break;
	}
	case OBJECT_TYPE_INTRO_CURTAIN: {
		obj = new Curtain(x, y);
		break;
	}
	case OBJECT_TYPE_MUSHROOM: {
		float model = (float)atof(tokens[4].c_str());
		obj = new Mushroom(x, y, model);
		break;
	}
	case OBJECT_TYPE_LEAF: {
		obj = new Leaf(x, y);
		break;
	}
	case OBJECT_TYPE_INTRO_ARROW: 
	{
		obj = new Arrow(x, y);
		mainArrow = (Arrow*)obj;
		break;
	}
	case OBJECT_TYPE_INTRO_SHININGTHREE: 
	{
		obj = new ShiningThree(x, y);

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


void IntroScene::Load()
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
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void IntroScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	CGameObject* obj = NULL;

	vector<LPGAMEOBJECT> coObjects;

	introTime += dt;

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT e = objects[i];
		if (introTime > 0 && introTime < 5000) {
			if (e->GetType() == CURTAIN)
			{
				Curtain* curtain = dynamic_cast<Curtain*>(e);
				curtain->Update(dt, &coObjects);
			}
		}
		else if (introTime > 5900 && introTime < 8000) {
			if (e->GetType() == MARIO)
			{
				CMario* mario = dynamic_cast<CMario*>(e);
				mario->Update(dt, &coObjects);
				if (introTime > 7900) {
					mario->SetState(MARIO_STATE_IDLE);
				}
				else mario->SetState(MARIO_STATE_WALK_LEFT);
			}
			if (e->GetType() == GOOMBA) {
				CGoomba* goomba = dynamic_cast<CGoomba*>(e);
				goomba->Update(dt, &coObjects);
			}
			if (e->GetType() == CURTAIN)
			{
				Curtain* curtain = dynamic_cast<Curtain*>(e);
				curtain->Update(dt, &coObjects);
			}
		}
		else if (introTime > 8000) {
			objects[i]->Update(dt, &coObjects);
			if (e->GetType() == KOOPAS) {
				CKoopas* koopa = dynamic_cast<CKoopas*>(e);
				koopa->SetState(KOOPAS_STATE_DEFEND);
			}
			if (e->GetType() == MUSHROOM_1_UP) {
				Mushroom* mushroom = dynamic_cast<Mushroom*>(e);
				mushroom->vy += 0.001f * dt;
			}
			if (e->GetType() == MUSHROOM_POWER) {	//red
				Mushroom* mushroom = dynamic_cast<Mushroom*>(e);
				mushroom->vy += 0.07f * dt;
			}
			if (e->GetType() == LEAF) {
				Leaf* leaf = dynamic_cast<Leaf*>(e);
				if (leaf->x <= leaf->limitLeft)
				{
					leaf->vx = LEAF_SPEED_X * dt;
				}
				if (leaf->x >= leaf->limitRight)
				{
					leaf->vx = -LEAF_SPEED_X * dt;
				}
				leaf->vy = 0.005f * dt;
			}
			if (e->GetType() == MARIO)
			{
				CMario* mario = dynamic_cast<CMario*>(e);
				if (introTime < 9700) {
					mario->vx = 0;
					mario->SetState(MARIO_STATE_IDLE);
					if (introTime > 9500) {
						mario->SetState(MARIO_STATE_JUMP);
					}
				}
				if (introTime > 10000 && introTime < 11000) {
					mario->SetState(MARIO_STATE_WALK_LEFT);
				}
				if (introTime > 11000) {
					mario->SetState(MARIO_STATE_WALK_RIGHT);
				}
			}
			if (e->GetType() == KOOPAS) {
				CKoopas* koopa = dynamic_cast<CKoopas*>(e);
				if (introTime > 10500 && introTime < 11500) {
					koopa->SetState(KOOPAS_STATE_BALL);
				}
			}
		}
		if (objects.at(i)->isFinish)
			objects.erase(objects.begin() + i);
	}
	if (player == NULL) return;
}

void IntroScene::Render()
{
	for (size_t i = 0; i < objects.size(); i++) {
		LPGAMEOBJECT e = objects[i];
		if (introTime > 0 && introTime < 8000) {
			if (e->GetType() == CURTAIN)
			{
				Curtain* curtain = dynamic_cast<Curtain*>(e);
				curtain->Render();
			}
			if (e->GetType() == GROUND)
			{
				Ground* ground = dynamic_cast<Ground*>(e);
				ground->Render();
			}
			if (e->GetType() == MARIO)
			{
				CMario* mario = dynamic_cast<CMario*>(e);
				mario->Render();
			}
			if (introTime >6000)
				if (e->GetType() == GOOMBA) {
					CGoomba* goomba = dynamic_cast<CGoomba*>(e);
					goomba->Render();
				}
		}
		else if (introTime > 8000 && introTime < 14000) {
			if (e->GetType() != ARROW && e->GetType() != SHININGTHREE)
				objects[i]->Render();
		}
		else if (introTime > 14000)
			objects[i]->Render();
	}
}

/*
	Unload current scene
*/
void IntroScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void IntroSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CGame *game = CGame::GetInstance();
	Arrow *arrow = ((IntroScene*)scence)->mainArrow;
	if (CGame::GetInstance()->GetScene() != WORLDMAP) {
		switch (KeyCode)
		{
		case DIK_DOWN:
			arrow->y = TWO_PLAYER_POS;
			break;
		case DIK_UP:
			arrow->y = ONE_PLAYER_POS;
			break;
		case DIK_S:
			if (arrow->y == ONE_PLAYER_POS)
				CGame::GetInstance()->SwitchScene(WORLDMAP);
			break;
		}
	}
}

void IntroSceneKeyHandler::KeyState(BYTE *states)
{

}

void IntroSceneKeyHandler::OnKeyUp(int KeyCode) {

}