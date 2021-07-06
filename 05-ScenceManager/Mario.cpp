#include"Mario.h"
#include "EffectPoint.h"
#include "EffectDisappear.h"
#include "EffectTailHit.h"
#include "EffectBrokenBrick.h"
#include "PlayScence.h"

CMario::CMario(float x, float y) 
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	eType = Type::MARIO;

}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGame *game = CGame::GetInstance();
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (vx * nx < 0 && !isJumping) {
		SetState(MARIO_STATE_TURN);
	}
	// Simple fall down
	if (CGame::GetInstance()->GetScene() != WORLDMAP)
	{
		vy += MARIO_GRAVITY * dt;		
	}
	else SetState(MARIO_STATE_WORLD_MAP);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	//-----------------------------FIRE BALL-------------------------------

	if (state == MARIO_STATE_SHOOT_FIRE) {
		if (listFire.size() < 2) {
			FireBall *fire = new FireBall(x + 5, y + 5, FIRE_SPEED, FIRE_GRAVITY);
			fire->nx = nx;
			listFire.push_back(fire);
		}
	}
	for (size_t i = 0; i < listFire.size(); i++)
	{
		listFire[i]->Update(dt, coObjects);
		if (!listFire[i]->CheckObjectInCamera() || listFire.at(i)->isFinish) {
			listFire.erase(listFire.begin() + i);
		}
	}
	for (size_t i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Update(dt, coObjects);
		if (!listEffect[i]->CheckObjectInCamera() || listEffect.at(i)->isFinish)
			listEffect.erase(listEffect.begin() + i);
	}
	//------------------------------------------------------------------
	if (GetTickCount() - untouchable_start > timeUntouchable)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount() - turning_start > TIME_SPINNING && isSpinning)
	{
		turning_start = 0;
		isSpinning = false;
	}
	if (GetTickCount() - limitfly_start > MARIO_LIMIT_FLY_TIME)
	{
		isFlying = false;
		limitfly_start = 0;
	}
	if (GetTickCount() - limitjump_start > marioLimitJumpTime)
	{
		isJumping = false;
		limitjump_start = 0;
	}

	if (isTransformToBig) {
		timeTransform += dt;
		if (timeTransform < TIME_TRANSFORM) {
			if (timeTransform % 2 == 0)
				SetLevel(MARIO_LEVEL_BIG);
			else SetLevel(MARIO_LEVEL_SMALL);
		}
		else {
			SetLevel(MARIO_LEVEL_BIG);
			isTransformToBig = false;
			timeTransform = 0;
		}

	}

	if (isTransformToRacoon) {
		timeTransform += dt;
		if (timeTransform < 200) {
			if (timeTransform % 2 == 0)
				SetLevel(MARIO_LEVEL_RACOON);
			else SetLevel(MARIO_LEVEL_BIG);
		}
		else {
			SetLevel(MARIO_LEVEL_RACOON);
			isTransformToRacoon = false;
			timeTransform = 0;
		}

	}
	//--------------------Check collision
	

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
	   //if (rdx != 0 && rdx!=dx)
	   //	x += nx*abs(rdx); 

	   // block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//
		if (CGame::GetInstance()->GetScene() != INTRO) {
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (e->obj->GetType() == GOOMBA) // if e->obj is Goomba 
				{
					CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

					if (e->ny < 0)
					{
						if (untouchable == 0) {
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								if (goomba->Health == 2) {
									goomba->Health = 1;
									if (goomba->model == GOOMBA_MODEL_WING_BROWN)
										goomba->model = 1;
									ShowEffectPoint(goomba, POINT_EFFECT_MODEL_100);
									PlusScore(100);
								}
								else
								{
									goomba->SetState(GOOMBA_STATE_DIE);
								}
								this->vy = -MARIO_JUMP_DEFLECT_SPEED;
							}
						}
					}
					else if (e->nx != 0 || e->ny > 0)
					{
						if (isSpinning && level == MARIO_LEVEL_RACOON) {
							goomba->nx = this->nx;
							goomba->SetState(GOOMBA_STATE_DIE);
							EffectTailHit* effectTailHit = new EffectTailHit(goomba->x, goomba->y);
							//scene->TurnIntoUnit(effectTailHit);
							listEffect.push_back(effectTailHit);
						}
						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									if (level > MARIO_LEVEL_BIG) {
										level = MARIO_LEVEL_BIG;
										EffectDisappear* effectDisappear = new EffectDisappear(this->x, this->y);
										//scene->TurnIntoUnit(effectDisappear);
										listEffect.push_back(effectDisappear);
										StartUntouchable(TIME_UNTOUCHABLE_SHORT);
									}
									else
									{
										level = MARIO_LEVEL_SMALL;
										StartUntouchable(TIME_UNTOUCHABLE_LONG);
									}
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
						else { goomba->x += dx; this->x += dx;}
					}
				}
				else if (e->obj->GetType() == KOOPAS)
				{
					CKoopas *koopa = dynamic_cast<CKoopas *>(e->obj);
					if (e->ny < 0)
					{
						if (koopa->model == KOOPAS_MODEL_GREEN_WING) {
							koopa->model = 1;
							ShowEffectPoint(koopa, POINT_EFFECT_MODEL_100);
							PlusScore(100);
							if (koopa->GetState() == KOOPAS_STATE_FLY)
								koopa->SetState(KOOPAS_STATE_WALKING);
						}

						if (koopa->GetState() == KOOPAS_STATE_DEFEND) {
							vy = -MARIO_JUMP_DEFLECT_SPEED *4;
							koopa->SetState(KOOPAS_STATE_BALL);
						}
						else if (koopa->GetState() != KOOPAS_STATE_DIE)
						{
							ShowEffectPoint(koopa, POINT_EFFECT_MODEL_100);
							PlusScore(100);
							vy = -MARIO_JUMP_DEFLECT_SPEED * 5;
							koopa->SetState(KOOPAS_STATE_DEFEND);
						}
					}
					else if (nx != 0)
					{
						koopa->x += dx;
						if (game->IsKeyDown(DIK_Q) && koopa->state == KOOPAS_STATE_DEFEND) {
							this->isHolding = true;
							koopa->isBeingHeld = true;
						}
						else if (koopa->GetState() == KOOPAS_STATE_DEFEND)
						{
							this->SetState(MARIO_STATE_KICK);
							koopa->nx = this->nx;
							koopa->SetState(KOOPAS_STATE_BALL);
						}
						else if (isSpinning && level == MARIO_LEVEL_RACOON) {
							koopa->vx += this->nx * MARIO_DIE_DEFLECT_SPEED * 2;
							koopa->vy = -MARIO_DIE_DEFLECT_SPEED;
							koopa->SetState(KOOPAS_STATE_DEFEND);
							EffectTailHit* effectTailHit = new EffectTailHit(koopa->x, koopa->y);
							//scene->TurnIntoUnit(effectTailHit);
							listEffect.push_back(effectTailHit);
						}
						else if (untouchable == 0)
						{
							if (koopa->GetState() != KOOPAS_STATE_DIE)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									if (level > MARIO_LEVEL_BIG) {
										level = MARIO_LEVEL_BIG;
										EffectDisappear* effectDisappear = new EffectDisappear(this->x, this->y);
										//scene->TurnIntoUnit(effectDisappear);
										listEffect.push_back(effectDisappear);
										StartUntouchable(TIME_UNTOUCHABLE_SHORT);
									}
									else
									{
										level = MARIO_LEVEL_SMALL;
										StartUntouchable(TIME_UNTOUCHABLE_LONG);
									}
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
				else if (e->obj->GetType() == COLORBLOCK)
				{
					if (e->ny > 0) {
						y += dy;
					}
				}
				else if (e->obj->GetType() == BRICK)
				{
					CBrick* brick = dynamic_cast<CBrick*>(e->obj);
					if (e->ny > 0) {
						if (brick->isUnbox == 0)
							brick->isUnbox = 1;
						y += dy; x -= dx;
						x -= this->nx * 8;// day mario ra xiu
						//vx -= this->nx * MARIO_DIE_DEFLECT_SPEED * dt;
					}
				}
				else if (e->obj->GetType() == PLATFORM) {
					isOnGround = true;
				}
				else if (e->obj->GetType() == FIRE) { // if e->obj is fireball 
					if (untouchable == 0)
					{
						if (level > MARIO_LEVEL_SMALL)
						{
							level = MARIO_LEVEL_SMALL;
							StartUntouchable(TIME_UNTOUCHABLE_LONG);
						}
						else
							SetState(MARIO_STATE_DIE);
					}
				}
				else if (e->obj->GetType() == FIREPLANT) { // if e->obj is fireball 
					FirePlant *plant = dynamic_cast<FirePlant *>(e->obj);
					if (untouchable == 0)
					{
						if (isSpinning && level == MARIO_LEVEL_RACOON) {
							plant->isFinish = true;
							EffectTailHit* effectTailHit = new EffectTailHit(plant->x, plant->y);
							//scene->TurnIntoUnit(effectTailHit);
							listEffect.push_back(effectTailHit);
						}
						if (level > MARIO_LEVEL_SMALL)
						{
							if (level > MARIO_LEVEL_BIG) {
								level = MARIO_LEVEL_BIG;
								EffectDisappear* effectDisappear = new EffectDisappear(this->x, this->y);
								listEffect.push_back(effectDisappear);
								StartUntouchable(TIME_UNTOUCHABLE_SHORT);
							}
							else
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable(TIME_UNTOUCHABLE_LONG);
							}
						}
						else
							SetState(MARIO_STATE_DIE);
					}
					else { this->x += dx; this->y += dy; }
				}
				else if (e->obj->GetType() == QUESTIONBRICK) { // if e->obj is fireball 
					QuestionBrick* qb = dynamic_cast<QuestionBrick*>(e->obj);
					if (e->ny > 0)
					{
						if (qb->Health == 1)
						{
							qb->vy = -QUESTION_BRICK_SPEED_UP * dt;
							qb->Health = 0;
							qb->isUnbox = true;
						}
						this->vy = MARIO_DIE_DEFLECT_SPEED;
					}
				}
				else if (e->obj->GetType() == GOLDBRICK) { 
					GoldBrick* gb = dynamic_cast<GoldBrick*>(e->obj);
					int model = (int)gb->model;
					if (e->ny > 0)
					{
						switch (model) {
						case GOLD_BRICK_MODEL_COIN:
							if (gb->y >= gb->minY) {								
							}
							gb->vy = -QUESTION_BRICK_SPEED_UP;
							break;
						case GOLD_BRICK_MODEL_PSWITCH:
							gb->SetState(GOLD_BRICK_STATE_UNBOX);
							break;
						}
						this->vy = MARIO_DIE_DEFLECT_SPEED;
					}
				}
				else if (e->obj->GetType() == MUSICBRICK) {
					MusicBrick* mb = dynamic_cast<MusicBrick*>(e->obj);
					if (e->ny < 0)//nhay tu duoi len 
					{
						if (mb->GetModel() == MUSIC_BRICK_MODEL_HIDDEN) {
							if (mb->isHidden) { x += dx; y += dy; }
							else if (game->IsKeyDown(DIK_SPACE)) {
								this->vy -= QUESTION_BRICK_SPEED_UP * 2 * dt;
								mb->vy = QUESTION_BRICK_SPEED_UP;
							}
						}
						else {
							this->vy = -(MARIO_DIE_DEFLECT_SPEED -0.01f) * dt;
							mb->vy = QUESTION_BRICK_SPEED_UP;
						}
					}
					else if (e->ny > 0) {//nhay tu tren xuong
						if (mb->GetModel() == MUSIC_BRICK_MODEL_HIDDEN)
							mb->isHidden = false;
						else {
							mb->vy = -QUESTION_BRICK_SPEED_UP;
							this->vy = MARIO_DIE_DEFLECT_SPEED;
						}
					}
				}
				else if (e->obj->GetType() == COIN) {
					//Coin* coin = dynamic_cast<Coin*>(e->obj);
					this->x += dx;
					e->obj->isFinish = true;
					PlusCoinCollect(1);
					PlusScore(50);
				}
				else if (e->obj->GetType() == PSWITCH)
				{
					PSwitch* pswitch = dynamic_cast<PSwitch*>(e->obj);
					if (e->ny < 0)
					{
						pswitch->SetState(PSWITCH_STATE_USED);
						pswitch->SetPosition(pswitch->x, pswitch->y + PSWITCH_SMALLER);
					}
				}
				else if (e->obj->GetType() == MUSHROOM_POWER || e->obj->GetType() == MUSHROOM_1_UP)
				{
					Mushroom* mushroom = dynamic_cast<Mushroom*>(e->obj);
					mushroom->isFinish = true;
					ShowEffectPoint(this, POINT_EFFECT_MODEL_1K);
					PlusScore(1000);
					if (e->obj->GetType() == MUSHROOM_POWER) {
						this->y -= 20;
						if (this->level < MARIO_LEVEL_BIG)
							this->SetLevel(MARIO_LEVEL_BIG);
					}
					//this->x += dx; //this->y += dy;
				}
				else if (e->obj->GetType() == LEAF)
				{
					Leaf* leaf = dynamic_cast<Leaf*>(e->obj);
					leaf->isFinish = true; this->y -= 5;
					ShowEffectPoint(this, POINT_EFFECT_MODEL_1K);
					PlusScore(1000);
					this->isTransformToRacoon = true;
					this->SetLevel(MARIO_LEVEL_RACOON);
				}
				else if (e->obj->GetType() == PORTAL) {

					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
					x += dx; y += dy;
					//if (p->GetSceneId() == SCENE_TEST  || p->GetSceneId() == MAP1_3) {
					//	CGame *game = CGame::GetInstance();
					//	if(game->IsKeyDown(DIK_E) && CheckAABB(p))
					//		CGame::GetInstance()->SwitchScene(p->GetSceneId());
					//}
					//else {
					//	CGame::GetInstance()->SwitchScene(p->GetSceneId());
					//}

				}
				else if (e->obj->GetType() == BOX) {
					Box* box = dynamic_cast<Box*>(e->obj);
					this->x += dx; this->y += dy;
					switch (box->GetState())
					{
					case BOX_STATE_FLOWER:
						box->stateUnbox = box->GetState();
						this->gotCard = box->stateUnbox;
						box->isUnbox = true;
						box->SetState(BOX_STATE_FLOWER_UP);
						break;
					case BOX_STATE_MUSHROOM:
						box->stateUnbox = box->GetState();
						this->gotCard = box->stateUnbox;
						box->isUnbox = true;
						box->SetState(BOX_STATE_MUSHROOM_UP);
						break;
					case BOX_STATE_STAR:
						box->stateUnbox = box->GetState();
						this->gotCard = box->stateUnbox;
						box->isUnbox = true;
						box->SetState(BOX_STATE_MUSHROOM_UP);
						break;
					}
					box->x += 2; //nhich box qua 1 xiu bay len cho dep
					box->vy = -MARIO_GRAVITY * 2 * dt;
				}
				else if (e->obj->GetType() == BOOMERANGBROTHER) {
					BoomerangBrother *bb = dynamic_cast<BoomerangBrother *>(e->obj);
					if (e->ny < 0)
					{
						if (bb->GetState() != BOOM_BROTHER_STATE_DIE)
						{
							bb->SetState(BOOM_BROTHER_STATE_DIE);
							bb->ListBoomerang.clear();
						}
					}
					else if (nx != 0)
					{
						if (isSpinning && level == MARIO_LEVEL_RACOON) {
							bb->vx += this->nx * MARIO_DIE_DEFLECT_SPEED * 2;
							bb->vy = -MARIO_DIE_DEFLECT_SPEED;
							bb->SetState(BOOM_BROTHER_STATE_DIE);
							EffectTailHit* effectTailHit = new EffectTailHit(bb->x, bb->y);
							listEffect.push_back(effectTailHit);
						}
						else if (untouchable == 0)
						{
							if (bb->GetState() != BOOM_BROTHER_STATE_DIE)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									if (level > MARIO_LEVEL_BIG) {
										level = MARIO_LEVEL_BIG;
										EffectDisappear* effectDisappear = new EffectDisappear(this->x, this->y);
										listEffect.push_back(effectDisappear);
										StartUntouchable(TIME_UNTOUCHABLE_SHORT);
									}
									else
									{
										level = MARIO_LEVEL_SMALL;
										StartUntouchable(TIME_UNTOUCHABLE_LONG);
									}
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
				else if (e->obj->GetType() == BOOMERANG) {
					//Boomerang *boomerang = dynamic_cast<Boomerang *>(e->obj);
					if (untouchable == 0)
					{
						if (level > MARIO_LEVEL_SMALL)
						{
							if (level > MARIO_LEVEL_BIG) {
								level = MARIO_LEVEL_BIG;
								EffectDisappear* effectDisappear = new EffectDisappear(this->x, this->y);
								listEffect.push_back(effectDisappear);
								StartUntouchable(TIME_UNTOUCHABLE_SHORT);
							}
							else
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable(TIME_UNTOUCHABLE_LONG);
							}
						}
						else SetState(MARIO_STATE_DIE);
					}
				}
				else if (e->obj->GetType() == POOP) {
					Poop* poop = dynamic_cast<Poop *>(e->obj);
					poop->isAttach = true;
					this->marioLimitJumpTime = 100;
				}
			}
		}
		else {//Collision Intro
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->obj->GetType() == GOOMBA) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				if (e->ny < 0)
				{
					if (untouchable == 0) {
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (goomba->Health == 2) {
								goomba->Health = 1;
								if (goomba->model == GOOMBA_MODEL_WING_BROWN)
									goomba->model = 1;
							}
							else
							{
								goomba->SetState(GOOMBA_STATE_DIE);
							}
							this->vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
				}
				else if (e->nx != 0 || e->ny > 0)
				{

					if (state == MARIO_STATE_SPIN && level == MARIO_LEVEL_RACOON) {
						goomba->nx = this->nx;
						goomba->SetState(GOOMBA_STATE_DIE);
					}
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								if (level > MARIO_LEVEL_BIG) {
									level = MARIO_LEVEL_BIG;
									StartUntouchable(TIME_UNTOUCHABLE_SHORT);
								}
								else
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable(TIME_UNTOUCHABLE_LONG);
								}
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
				else if (e->obj->GetType() == KOOPAS)
				{
					CKoopas *koopa = dynamic_cast<CKoopas *>(e->obj);
					x += dx;
				}
				else if (e->obj->GetType() == PLATFORM) {
				isOnGround = true;
			}
				else if (e->obj->GetType() == MUSHROOM_POWER || e->obj->GetType() == MUSHROOM_1_UP)
				{
					Mushroom* mushroom = dynamic_cast<Mushroom*>(e->obj);
					mushroom->isFinish = true;
					if (e->obj->GetType() == MUSHROOM_POWER) {
						this->y -= 20;
						if (this->level < MARIO_LEVEL_BIG)
							this->SetLevel(MARIO_LEVEL_BIG);
					}
				//this->x += dx; //this->y += dy;
				}
				else if (e->obj->GetType() == LEAF)
				{
					Leaf* leaf = dynamic_cast<Leaf*>(e->obj);
					leaf->isFinish = true; this->y -= 5;
					this->isTransformToRacoon = true;
					this->SetLevel(MARIO_LEVEL_RACOON);
				}
			}
		}		
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
		int ani = -1;
		if (state == MARIO_STATE_DIE)
			ani = MARIO_ANI_DIE;
		//-----------MARIO ---------------------
		else if (level == MARIO_LEVEL_BIG) //MARIO BIG
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_BIG_WALK_RIGHT;
			else ani = MARIO_ANI_BIG_WALK_LEFT;

			if (state == MARIO_STATE_TURN)
			{
				if (nx < 0)
					ani = MARIO_ANI_BIG_TURN_LEFT;

				if (nx > 0)
					ani = MARIO_ANI_BIG_TURN_RIGHT;
			}

			if (state == MARIO_STATE_RUN_RIGHT || (isFlying && nx > 0))
				ani = MARIO_ANI_BIG_RUN_RIGHT;
			if (state == MARIO_STATE_RUN_LEFT || (isFlying && nx < 0))
				ani = MARIO_ANI_BIG_RUN_LEFT;

			if (state == MARIO_STATE_RUN_MAXSPEED) {
				if (nx > 0)
					ani = MARIO_ANI_BIG_RUN_RIGHT_MAXSPEED;
				else ani = MARIO_ANI_BIG_RUN_LEFT_MAXSPEED;
			}

			if (state == MARIO_STATE_FLY_RIGHT || isFlying)
				ani = MARIO_ANI_BIG_FLY_RIGHT;
			if (state == MARIO_STATE_FLY_LEFT || isFlying)
				ani = MARIO_ANI_BIG_FLY_LEFT;

			if (state == MARIO_STATE_JUMP || !isOnGround) {
				if (nx > 0) ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else ani = MARIO_ANI_BIG_JUMP_LEFT;				
			}

			if (state == MARIO_STATE_SIT) {
				if (nx > 0) ani = MARIO_ANI_BIG_SIT_RIGHT;
				else ani = MARIO_ANI_BIG_SIT_LEFT;
			}
			if (state == MARIO_STATE_KICK) {
				if (nx > 0) ani = MARIO_ANI_BIG_KICK_RIGHT;
				else ani = MARIO_ANI_BIG_KICK_LEFT;
			}
			if (isHolding) {
				if (vx == 0)	//MARIO_STATE_WALKING
				{
					if (nx > 0) ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
					else ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
				}
				else if (vx > 0)
					ani = MARIO_ANI_BIG_HOLD_WALK_RIGHT;
				else ani = MARIO_ANI_BIG_HOLD_WALK_LEFT;
			}
			if (vy < 0 && !isFlying) {
				if (nx > 0)ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else ani = MARIO_ANI_BIG_JUMP_LEFT;
			}
			if (state == MARIO_STATE_WORLD_MAP)
				ani = MARIO_ANI_BIG_WORLD_MAP;
		}

		//-----------MARIO SMALL---------------------
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_SMALL_WALK_RIGHT;
			else ani = MARIO_ANI_SMALL_WALK_LEFT;

			if (state == MARIO_STATE_TURN)
			{
				if (nx < 0)
					ani = MARIO_ANI_SMALL_TURN_LEFT;

				if (nx > 0)
					ani = MARIO_ANI_SMALL_TURN_RIGHT;
			}

			if (state == MARIO_STATE_RUN_RIGHT)
				ani = MARIO_ANI_SMALL_RUN_RIGHT;
			if (state == MARIO_STATE_RUN_LEFT)
				ani = MARIO_ANI_SMALL_RUN_LEFT;

			if (state == MARIO_STATE_RUN_MAXSPEED) {
				if (nx > 0)
					ani = MARIO_ANI_SMALL_RUN_RIGHT_MAXSPEED;
				else ani = MARIO_ANI_SMALL_RUN_LEFT_MAXSPEED;
			}

			if (state == MARIO_STATE_FLY_RIGHT)
				ani = MARIO_ANI_SMALL_FLY_RIGHT;
			if (state == MARIO_STATE_FLY_LEFT)
				ani = MARIO_ANI_SMALL_FLY_LEFT;

			if (state == MARIO_STATE_JUMP && isJumping) {
				if (nx > 0) ani = MARIO_ANI_SMALL_JUMP_RIGHT;
				else ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
			if (state == MARIO_STATE_KICK) {
				if (nx > 0) ani = MARIO_ANI_SMALL_KICK_RIGHT;
				else ani = MARIO_ANI_SMALL_KICK_LEFT;
			}
			if (isHolding) {
				if (vx == 0)	//MARIO_STATE_WALKING
				{
					if (nx > 0) ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
					else ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
				}
				else if (vx > 0)
					ani = MARIO_ANI_SMALL_HOLD_WALK_RIGHT;
				else ani = MARIO_ANI_SMALL_HOLD_WALK_LEFT;
			}
			if (vy < 0) {
				if (nx > 0)ani = MARIO_ANI_SMALL_JUMP_RIGHT;
				else ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
			if (state == MARIO_STATE_WORLD_MAP)
				ani = MARIO_ANI_SMALL_WORLD_MAP;
		}
		//-----------MARIO FIRE---------------------
		else if (level == MARIO_LEVEL_FIRE) //----------------------------------------MARIO FIRE----------------------------------------
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_FIRE_WALK_RIGHT;
			else ani = MARIO_ANI_FIRE_WALK_LEFT;

			if (state == MARIO_STATE_TURN)
			{
				if (nx < 0)
					ani = MARIO_ANI_FIRE_TURN_LEFT;

				if (nx > 0)
					ani = MARIO_ANI_FIRE_TURN_RIGHT;
			}

			if (state == MARIO_STATE_RUN_RIGHT)
				ani = MARIO_ANI_FIRE_RUN_RIGHT;
			if (state == MARIO_STATE_RUN_LEFT)
				ani = MARIO_ANI_FIRE_RUN_LEFT;

			if (state == MARIO_STATE_RUN_MAXSPEED)
				if (nx > 0)
					ani = MARIO_ANI_FIRE_RUN_RIGHT_MAXSPEED;
				else ani = MARIO_ANI_FIRE_RUN_LEFT_MAXSPEED;

			if (state == MARIO_STATE_FLY_RIGHT)
				ani = MARIO_ANI_FIRE_FLY_RIGHT;
			if (state == MARIO_STATE_FLY_LEFT)
				ani = MARIO_ANI_FIRE_FLY_LEFT;


			if (state == MARIO_STATE_JUMP && isJumping) {
				if (nx > 0) ani = MARIO_ANI_FIRE_JUMP_RIGHT;
				else ani = MARIO_ANI_FIRE_JUMP_LEFT;
			}

			if (state == MARIO_STATE_SIT) {
				if (nx > 0) ani = MARIO_ANI_FIRE_SIT_RIGHT;
				else ani = MARIO_ANI_FIRE_SIT_LEFT;
			}
			if (state == MARIO_STATE_KICK) {
				if (nx > 0) ani = MARIO_ANI_FIRE_KICK_RIGHT;
				else ani = MARIO_ANI_FIRE_KICK_LEFT;
			}
			if (state == MARIO_STATE_SHOOT_FIRE) {
				if (nx > 0) ani = MARIO_ANI_FIRE_SHOOT_FIRE_RIGHT;
				else ani = MARIO_ANI_FIRE_SHOOT_FIRE_LEFT;
			}
			if (vy < 0) {
				if (nx > 0)ani = MARIO_ANI_FIRE_JUMP_RIGHT;
				else ani = MARIO_ANI_FIRE_JUMP_LEFT;
			}
			if (state == MARIO_STATE_WORLD_MAP)
				ani = MARIO_ANI_FIRE_WORLD_MAP;
		}
		//-----------MARIO RACOON---------------------
		else if (level == MARIO_LEVEL_RACOON)//----------------------------------------MARIO RACOON----------------------------------------
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_RACOON_IDLE_RIGHT;
				else ani = MARIO_ANI_RACOON_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_RACOON_WALK_RIGHT;
			else ani = MARIO_ANI_RACOON_WALK_LEFT;

			if (state == MARIO_STATE_TURN)
			{
				if (nx < 0)
					ani = MARIO_ANI_RACOON_TURN_LEFT;

				if (nx > 0)
					ani = MARIO_ANI_RACOON_TURN_RIGHT;
			}

			if (state == MARIO_STATE_RUN_RIGHT)
				ani = MARIO_ANI_RACOON_RUN_RIGHT;
			if (state == MARIO_STATE_RUN_LEFT)
				ani = MARIO_ANI_RACOON_RUN_LEFT;

			if (state == MARIO_STATE_RUN_MAXSPEED)
				if (nx > 0)
					ani = MARIO_ANI_RACOON_RUN_RIGHT_MAXSPEED;
				else ani = MARIO_ANI_RACOON_RUN_LEFT_MAXSPEED;

			if (state == MARIO_STATE_FLY_RIGHT)
				ani = MARIO_ANI_RACOON_FLY_RIGHT;
			if (state == MARIO_STATE_FLY_LEFT)
				ani = MARIO_ANI_RACOON_FLY_LEFT;


			if (state == MARIO_STATE_JUMP ) {
				if (nx > 0) ani = MARIO_ANI_RACOON_JUMP_RIGHT;
				else ani = MARIO_ANI_RACOON_JUMP_LEFT;
			}

			if (state == MARIO_STATE_SIT) {
				if (nx > 0) ani = MARIO_ANI_RACOON_SIT_RIGHT;
				else ani = MARIO_ANI_RACOON_SIT_LEFT;
			}

			if (state == MARIO_STATE_KICK) {
				if (nx > 0) ani = MARIO_ANI_RACOON_KICK_RIGHT;
				else ani = MARIO_ANI_RACOON_KICK_LEFT;
			}

			if (state == MARIO_STATE_SPIN || isSpinning) {
				if (nx > 0) ani = MARIO_ANI_RACOON_SPIN_RIGHT;
				else ani = MARIO_ANI_RACOON_SPIN_LEFT;
			}
			if (vy < 0) {
				if (nx > 0)ani = MARIO_ANI_RACOON_JUMP_RIGHT;
				else ani = MARIO_ANI_RACOON_JUMP_LEFT;
			}
			if (state == MARIO_STATE_WORLD_MAP)
				ani = MARIO_ANI_RACOON_WORLD_MAP;

		}
		int alpha = 255;
		if (untouchable) {
			if (timeUntouchable == TIME_UNTOUCHABLE_SHORT)
				alpha = 0;
			else alpha = 128;
		}			
		animation_set->at(ani)->Render(x, y, alpha);

		for (size_t i = 0; i < listFire.size(); i++)
		{
			listFire[i]->Render();
		}

		for (size_t i = 0; i < listEffect.size(); i++)
		{
			listEffect[i]->Render();
		}
		RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_IDLE:
		vx = 0;		
		isSitting = false;
		isOnGround = true;
		break;
	case MARIO_STATE_WALK_RIGHT:
		nx = 1;
		vx += MARIO_WALKING_SPEED * dt;
		if (abs(vx) >= MARIO_WALKING_MAXSPEED)
			vx = MARIO_WALKING_MAXSPEED;
		break;
	case MARIO_STATE_WALK_LEFT:
		nx = -1;
		vx -= MARIO_WALKING_SPEED * dt;
		if (abs(vx) >= MARIO_WALKING_MAXSPEED)
			vx = -MARIO_WALKING_MAXSPEED;
		break;
	case MARIO_STATE_JUMP:
		if (isJumping) {
			vy = -MARIO_JUMP_SPEED * dt;
		}
		break; 
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_SIT:
		isSitting = true;
		break;
	case MARIO_STATE_TURN:
		break;
	case MARIO_STATE_RUN_RIGHT:
		nx = 1;
		vx += MARIO_RUNNING_SPEED * dt;
		if (abs(vx) >= MARIO_RUNNING_MAXSPEED) {
			SetState(MARIO_STATE_RUN_MAXSPEED);
		}
		break;
	case MARIO_STATE_RUN_LEFT:
		nx = -1;
		vx -= MARIO_RUNNING_SPEED * dt;
		if (abs(vx) >= MARIO_RUNNING_MAXSPEED) {
			SetState(MARIO_STATE_RUN_MAXSPEED);
		}
		break;
	case MARIO_STATE_FLY_RIGHT:
		if (isFlying) {
			vx += MARIO_RUNNING_SPEED * dt;
			vy = -MARIO_JUMP_SPEED * dt;
		}
		break;
	case MARIO_STATE_FLY_LEFT:
		if (isFlying) {
			vx -= MARIO_RUNNING_SPEED * dt;
			vy = -MARIO_JUMP_SPEED * dt;
		}
		break;
	case MARIO_STATE_KICK:
	case MARIO_STATE_SHOOT_FIRE:
	case MARIO_STATE_SPIN:
	case MARIO_STATE_HOLD:
	case MARIO_STATE_RUN_MAXSPEED:
	case MARIO_STATE_WORLD_MAP:
		break;
	}

}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE || level == MARIO_LEVEL_RACOON)
	{
		if (state == MARIO_STATE_SIT) {
			right = x + MARIO_BIG_BBOX_SIT_WIDTH;
			bottom = y + MARIO_BIG_BBOX_SIT_HEIGHT;
		}
		else if (state == MARIO_STATE_SPIN || isSpinning){
			right = x + MARIO_RACOON_BBOX_SPIN_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		else{
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	if (state == MARIO_STATE_WORLD_MAP) {
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::ShowEffectPoint(CGameObject* obj, float model) {
	EffectPoint* effectPoint = new EffectPoint(obj->x, obj->y, model);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	listEffect.push_back(effectPoint);
	//scene->TurnIntoUnit(effectPoint);
}


