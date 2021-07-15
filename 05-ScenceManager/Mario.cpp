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
	ax = MARIO_ACCELERATION;
	ay = MARIO_GRAVITY;
	nx = 1;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	RunningStacks = 0;
	eType = Type::MARIO;
}

void CMario::TimingFlag()
{

	if (GetTickCount() - slow_start > MARIO_SLOW_TIME && slow_start != 0)
	{
		slow_start = 0;
		isReadyToRun = true;
	}
	if (GetTickCount() - running_start > MARIO_RUNNING_STACK_TIME && isRunning && vx != 0 && isReadyToRun)
	{
		running_start = GetTickCount();
		RunningStacks++;

		if (RunningStacks > 7)
		{
			RunningStacks = 7;
			vx = nx * MARIO_RUNNING_MAXSPEED;
		}
	}
	if (GetTickCount() - running_stop > MARIO_SLOW_STACK_TIME && !isRunning)
	{
		running_stop = GetTickCount();
		RunningStacks--;
		if (RunningStacks < 0)
		{
			RunningStacks = 0;
			isRunning = false;
			isFlying = false;
		}
	}
	if (GetTickCount() - kicking_start > MARIO_KICKING_TIME && isKicking)	isKicking = false;
	if (GetTickCount() - shooting_start > MARIO_SHOOTING_TIME && isShooting) isShooting = false;
	if (GetTickCount() - turning_state_start > MARIO_TURNING_STATE_TIME && isTurningTail)
	{
		turning_state_start = GetTickCount();
		turning_state++;
	}
	if (GetTickCount() - turning_start > MARIO_TURNING_TAIL_TIME && isTurningTail)
	{
		isTurningTail = false;
		turning_state = 0;
		turning_state_start = 0;
	}
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME && untouchable == 1)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount() - flapping_start > MARIO_FLAPPING_TIME && isFlapping)	isFlapping = false;
	if (GetTickCount() - flapping_start <= MARIO_FLAPPING_TIME && isFlapping)
	{
		if (vy >= MARIO_SLOW_FALLING_SPEED)
			vy = MARIO_SLOW_FALLING_SPEED;
	}
	if (GetTickCount() - tailflying_start > MARIO_TAIL_FLYING_TIME && isTailFlying)
	{
		isTailFlying = false;
		isRunning = false;
		isFlying = false;
	}
	if (GetTickCount() - fly_start > MARIO_FLYING_TIME && fly_start != 0 && isFlying)
	{
		fly_start = 0;
		isTailFlying = false;
		isRunning = false;
		isFlying = false;
		StartSlowDown();
	}
}
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGame *game = CGame::GetInstance();
	// Calculate dx, dy 
	CGameObject::Update(dt);
	//if (vx * nx < 0 && !isJumping) {
	//	SetState(MARIO_STATE_TURN);
	//}
	// Simple fall down
	if (CGame::GetInstance()->GetScene() != WORLDMAP)
	{
		if (vx * ax < 0 && abs(vx) > MARIO_WALKING_MAXSPEED
			&& (state == MARIO_STATE_WALK_LEFT || state == MARIO_STATE_WALK_RIGHT))
		{
			vx = -nx * MARIO_WALKING_MAXSPEED;
			if (RunningStacks < 0)
				RunningStacks = 0;
		}
		//update speed
		vx += ax * dt + RunningStacks * ax;
		vy += ay * dt;
		//limit the speed of mario 
		if (abs(vx) >= MARIO_WALKING_MAXSPEED)
		{
			if (!isRunning)
				vx = nx * MARIO_WALKING_MAXSPEED;
			else
				if (abs(vx) >= MARIO_RUNNING_MAXSPEED && vx * ax > 0)
					if (RunningStacks < 7)
						vx = nx * MARIO_RUNNING_SPEED;
					else
						vx = nx * MARIO_RUNNING_MAXSPEED;
		}
		if (vy > MARIO_JUMP_SPEED_MAX)
		{
			vy = MARIO_JUMP_SPEED_MAX;
			ay = MARIO_GRAVITY;
		}
		if (vy <= -limitJumpVelocity)
		{
			vy = -limitJumpVelocity;
			ay = MARIO_GRAVITY;
			isReadyToJump = false;
		}
		if (isFlying)
		{
			vy = -MARIO_FLY_SPEED;
			ay = -MARIO_GRAVITY;
			isReadyToJump = false;
		}
		//timing
		TimingFlag();
		//
		if (state == MARIO_STATE_SIT && vy < 0)
			vy -= MARIO_ACCELERATION_JUMP * dt;
		if (isJumping && isChangeDirection)
			vx = 0;
		if (vy < 0)
			isOnGround = false;
	}
	else SetState(MARIO_STATE_WORLD_MAP);

	if (this == NULL) return;
	
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
	//--------------------Check collision
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (y - lastStandingY > 1.0f)
			isOnGround = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		float oLeft, oTop, oRight, oBottom;
		float mLeft, mTop, mRight, mBottom;
		//
		// Collision logic with other objects
		//
		if (CGame::GetInstance()->GetScene() != INTRO) {
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (e->ny != 0) {
					isOnGround = true;
					isJumping = false;
					isChangeDirection = false;
					isReadyToSit = true;
					isFlying = false;
				}

				if (this != NULL && e->obj != NULL) {
					GetBoundingBox(mLeft, mTop, mRight, mBottom);
					e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);

				}
				
				if (e->obj->GetType() == PLATFORM) {
					if (e->ny < 0)
					{
						vy = 0;
						lastStandingY = y;
						
					}
					if (e->ny > 0)
					{
						vy = 0;
						ay = MARIO_GRAVITY;
						isReadyToJump = false;
					}
					if (e->nx != 0)
					{
						if (ceil(mBottom) != oTop)
						{
							vx = 0;
							if (isRunning)
								StopRunning();
							if (x < game->GetCamX())
							{
								SetState(MARIO_STATE_DIE);
								return;
							}
						}
					}
				}
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
						else { goomba->y -= ENEMY_PUSH_BACK; this->x += dx; }
					}
					else if (e->nx != 0 || e->ny > 0)
					{
						if (isTurningTail && level == MARIO_LEVEL_RACOON) {
							goomba->nx = this->nx;
							goomba->SetState(GOOMBA_STATE_DIE);
							EffectTailHit* effectTailHit = new EffectTailHit(goomba->x, goomba->y);
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
										listEffect.push_back(effectDisappear);
										StartUntouchable();
									}
									else
									{
										level = MARIO_LEVEL_SMALL;
										StartUntouchable();
									}
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
						else { goomba->x += goomba->nx * ENEMY_PUSH_BACK; this->x += dx; }
					}
				}
				else if (e->obj->GetType() == KOOPAS)
				{
					CKoopas *koopa = dynamic_cast<CKoopas *>(e->obj);
					if (e->ny < 0)
					{
						if (untouchable == 0) {
							e->obj->y -= ENEMY_PUSH_BACK;
							if (koopa->model == KOOPAS_MODEL_GREEN_WING) {
								koopa->model = KOOPAS_MODEL_GREEN;
								ShowEffectPoint(koopa, POINT_EFFECT_MODEL_100);
								PlusScore(100);
								if (koopa->GetState() == KOOPAS_STATE_FLY)
									koopa->SetState(KOOPAS_STATE_WALKING);
							}

							if (koopa->GetState() == KOOPAS_STATE_DEFEND) {
								vy = -MARIO_JUMP_DEFLECT_SPEED;
								koopa->SetState(KOOPAS_STATE_BALL);
							}
							else if (koopa->GetState() != KOOPAS_STATE_DIE)
							{
								ShowEffectPoint(koopa, POINT_EFFECT_MODEL_100);
								PlusScore(100);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
								koopa->SetState(KOOPAS_STATE_DEFEND);
							}
						}
						else { koopa->y -= ENEMY_PUSH_BACK; this->x += dx; }
					}
					else if (e->nx != 0)
					{
						if (game->IsKeyDown(DIK_A)) {
							if (isTurningTail && level == MARIO_LEVEL_RACOON) {

								if (koopa->GetState() == KOOPAS_STATE_WALKING)
									koopa->SetState(KOOPAS_STATE_DEFEND);

								EffectTailHit* effectTailHit = new EffectTailHit(koopa->x, koopa->y);
								listEffect.push_back(effectTailHit);
							}
							else if (koopa->state == KOOPAS_STATE_DEFEND || koopa->state == KOOPAS_STATE_UP_SIDE_DOWN) {
								this->isHolding = true;
								koopa->isBeingHeld = true;
							}
						}				
						else if (koopa->GetState() == KOOPAS_STATE_DEFEND)
						{
							this->SetState(MARIO_STATE_KICK);
							this->StartKicking();
							koopa->nx = this->nx;
							koopa->SetState(KOOPAS_STATE_BALL);
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
										listEffect.push_back(effectDisappear);
										StartUntouchable();
									}
									else
									{
										level = MARIO_LEVEL_SMALL;
										StartUntouchable();
									}
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
						else { this->x += dx; koopa->x += koopa->nx * ENEMY_PUSH_BACK + dx; }
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
				else if (e->obj->GetType() == FIRE) { // if e->obj is fireball 
					if (untouchable == 0)
					{
						if (level > MARIO_LEVEL_SMALL)
						{
							if (level > MARIO_LEVEL_BIG) {
								level = MARIO_LEVEL_BIG;
								EffectDisappear* effectDisappear = new EffectDisappear(this->x, this->y);
								listEffect.push_back(effectDisappear);
								StartUntouchable();
							}
							else
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
						}
						else
							SetState(MARIO_STATE_DIE);
					}
				}
				else if (e->obj->GetType() == FIREPLANT) {
					FirePlant *plant = dynamic_cast<FirePlant *>(e->obj);
					if (untouchable == 0)
					{
						if (isTurningTail && level == MARIO_LEVEL_RACOON) {
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
								StartUntouchable();
							}
							else
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
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
							qb->vy = -QUESTION_BRICK_SPEED_UP;
							qb->Health = 0;
							qb->isUnbox = true;
						}
						ay = MARIO_GRAVITY;
						isReadyToJump = false;
						this->vy = MARIO_JUMP_SPEED_MAX;
					}
					if (e->ny < 0)
					{
						vy = 0;
						lastStandingY = y;
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
						vy = 0;
						ay = MARIO_GRAVITY;
						isReadyToJump = false;
						this->vy = MARIO_JUMP_SPEED_MAX;
					}
					if (e->ny < 0)
					{
						vy = 0;
						lastStandingY = y;
					}
				}
				else if (e->obj->GetType() == MUSICBRICK) {
					MusicBrick* mb = dynamic_cast<MusicBrick*>(e->obj);
					if (e->ny < 0) //nhay tu tren xuong
					{
						if (mb->GetModel() == MUSIC_BRICK_MODEL_HIDDEN) {
							if (mb->isHidden) { x += dx; y += dy; }
							else {
								this->vy = -(MARIO_DIE_DEFLECT_SPEED)* dt;
								mb->vy = QUESTION_BRICK_SPEED_UP / 2;
								lastStandingY = y;
								if (game->IsKeyDown(DIK_S)) {
									this->vy -= MARIO_FLY_SPEED *dt; //nhay qua map phu
									mb->vy = QUESTION_BRICK_SPEED_UP;
								}
							}
						}
						else {
							this->vy = -(MARIO_DIE_DEFLECT_SPEED) * dt;
							mb->vy = QUESTION_BRICK_SPEED_UP/2;
							lastStandingY = y;
						}
						
					}
					else if (e->ny > 0) {//nhay tu duoi len
						if (mb->GetModel() == MUSIC_BRICK_MODEL_HIDDEN)
							mb->isHidden = false;
						else {
							mb->vy = -QUESTION_BRICK_SPEED_UP;
							vy = 0;
							ay = MARIO_GRAVITY;
							isReadyToJump = false;
							this->vy = MARIO_JUMP_SPEED_MAX;
						}					
					}
					if (e->nx != 0)
					{
						if (ceil(mBottom) != oTop)
						{
							vx = 0;
							if (isRunning)
								StopRunning();
							if (x < game->GetCamX())
							{
								SetState(MARIO_STATE_DIE);
								return;
							}
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
					//this->isTransformToRacoon = true;
					this->SetLevel(MARIO_LEVEL_RACOON);
				}
				else if (e->obj->GetType() == PORTAL) {
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					if (CGame::GetInstance()->GetScene() != WORLDMAP)
						CGame::GetInstance()->SwitchScene((int)p->GetSceneId());
					x += dx; y += dy;
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
						if (isTurningTail && level == MARIO_LEVEL_RACOON) {
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
										StartUntouchable();
									}
									else
									{
										level = MARIO_LEVEL_SMALL;
										StartUntouchable();
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
								StartUntouchable();
							}
							else
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
						}
						else SetState(MARIO_STATE_DIE);
					}
				}
				else if (e->obj->GetType() == POOP) {
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
									StartUntouchable();
								}
								else
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
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
					//this->isTransformToRacoon = true;
					this->SetLevel(MARIO_LEVEL_RACOON);
				}
			}
		}		
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::BasicRenderLogicsForAllLevel(int& ani,
	int ani_idle_right, int ani_idle_left, int ani_jump_down_right, int ani_jump_down_left,
	int ani_braking_left, int ani_braking_right, int ani_walking_right, int ani_walking_left, int ani_kicking_right, int ani_kicking_left) {

	if (vx == 0) {
		if (isOnGround) {
			if (nx > 0) ani = ani_idle_right;
			else ani = ani_idle_left;
			if(isKicking)
				if (nx > 0) ani = ani_kicking_right;
				else ani = ani_kicking_left;
		}
		else {
			if (nx > 0) ani = ani_jump_down_right;
			else ani = ani_jump_down_left;
		}
		if (isShooting)
		{
			if (nx > 0)
				ani = MARIO_ANI_FIRE_SHOOT_FIRE_RIGHT;
			else if (nx < 0)
				ani = MARIO_ANI_FIRE_SHOOT_FIRE_LEFT;
		}
	}
	else {
		if (vx > 0 && nx < 0) {
			ani = ani_braking_right;
		}
		else if (vx < 0 && nx >0) {
			ani = ani_braking_left;
		}
		else if (vx > 0 && nx > 0) {
			ani = ani_walking_right;
		}
		else if (vx < 0 && nx < 0) {
			ani = ani_walking_left;
		}
		if (isKicking)
		{
			if (vx > 0)	ani = ani_kicking_right;
			else if (vx < 0)	ani = ani_kicking_left;
		}
		if (vx != 0 && vy > 0 && !isOnGround) {
			if (nx > 0) ani = ani_jump_down_right;
			else ani = ani_jump_down_left;
		}
		if (isShooting)
		{
			if (nx > 0)
				ani = MARIO_ANI_FIRE_SHOOT_FIRE_RIGHT;
			else if (nx < 0)
				ani = MARIO_ANI_FIRE_SHOOT_FIRE_LEFT;
		}
	}
}

void CMario::RenderJumping(int& ani, int ani_jump_up_right, int ani_jump_up_left, int ani_jump_down_right, int ani_jump_down_left)
{
	if (nx > 0 && vy <= 0)
		ani = ani_jump_up_right;
	else if (nx < 0 && vy <= 0)
		ani = ani_jump_up_left;
	else if (nx > 0 && vy >= 0)
		ani = ani_jump_down_right;
	else if (nx < 0 && vy >= 0)
		ani = ani_jump_down_left;
}

void CMario::Render()
{
	int ani = -1;
	int alpha = 255;

	if (state == MARIO_STATE_DIE)	ani = MARIO_ANI_DIE;
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (isJumping)
		{
			if (!isHolding)
			{
				if (isFlying)
				{
					RenderJumping(ani,MARIO_ANI_SMALL_FLY_RIGHT,MARIO_ANI_SMALL_FLY_LEFT,MARIO_ANI_SMALL_FLY_RIGHT,MARIO_ANI_SMALL_FLY_LEFT);
				}
				else
					RenderJumping(ani,MARIO_ANI_SMALL_JUMP_RIGHT,MARIO_ANI_SMALL_JUMP_LEFT,MARIO_ANI_SMALL_JUMP_RIGHT,MARIO_ANI_SMALL_JUMP_LEFT);
			}
			else
				RenderJumping(ani,MARIO_ANI_SMALL_HOLD_JUMP_RIGHT,MARIO_ANI_SMALL_HOLD_JUMP_LEFT,MARIO_ANI_SMALL_HOLD_JUMP_RIGHT,MARIO_ANI_SMALL_HOLD_JUMP_LEFT);
		}
		else
		{
			if (!isHolding)
			{
				int ani_go_right = MARIO_ANI_SMALL_WALK_RIGHT;
				int ani_go_left = MARIO_ANI_SMALL_WALK_LEFT;
				if (isRunning)
					if (abs(vx) >= MARIO_RUNNING_MAXSPEED)
					{
						ani_go_right = MARIO_ANI_SMALL_RUN_RIGHT;
						ani_go_left = MARIO_ANI_SMALL_RUN_LEFT;
					}
				if (RunningStacks == 7)
				{
					ani_go_right = MARIO_ANI_SMALL_RUN_RIGHT_MAXSPEED;
					ani_go_left = MARIO_ANI_SMALL_RUN_LEFT_MAXSPEED;
				}
				BasicRenderLogicsForAllLevel(ani,
					MARIO_ANI_SMALL_IDLE_RIGHT, MARIO_ANI_SMALL_IDLE_LEFT,
					MARIO_ANI_SMALL_JUMP_RIGHT, MARIO_ANI_SMALL_JUMP_LEFT,
					MARIO_ANI_SMALL_TURN_RIGHT, MARIO_ANI_SMALL_TURN_LEFT,
					ani_go_right, ani_go_left, MARIO_ANI_SMALL_KICK_RIGHT, MARIO_ANI_SMALL_KICK_LEFT);
			}
			else
				BasicRenderLogicsForAllLevel(ani,
					MARIO_ANI_SMALL_HOLD_IDLE_RIGHT, MARIO_ANI_SMALL_HOLD_IDLE_LEFT,
					MARIO_ANI_SMALL_JUMP_RIGHT, MARIO_ANI_SMALL_JUMP_LEFT,
					MARIO_ANI_SMALL_TURN_RIGHT, MARIO_ANI_SMALL_TURN_LEFT,
					MARIO_ANI_SMALL_HOLD_WALK_RIGHT, MARIO_ANI_SMALL_HOLD_WALK_LEFT, MARIO_ANI_SMALL_KICK_RIGHT, MARIO_ANI_SMALL_KICK_LEFT);
		}
		if (state == MARIO_STATE_WORLD_MAP)
			ani = MARIO_ANI_SMALL_WORLD_MAP;

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
	else if (level == MARIO_LEVEL_BIG)
	{
		if (state == MARIO_STATE_SIT)
		{
			if (nx > 0)	ani = MARIO_ANI_BIG_SIT_RIGHT;
			else	ani = MARIO_ANI_BIG_SIT_LEFT;
		}
		else if (isJumping)
		{
			if (!isHolding)
			{
				if (isFlying)
				{
					RenderJumping(ani,MARIO_ANI_BIG_FLY_RIGHT,MARIO_ANI_BIG_FLY_LEFT,MARIO_ANI_BIG_FLY_RIGHT,MARIO_ANI_BIG_FLY_LEFT);
				}
				else
					RenderJumping(ani,MARIO_ANI_BIG_JUMP_RIGHT,MARIO_ANI_BIG_JUMP_LEFT,MARIO_ANI_BIG_JUMP_RIGHT,MARIO_ANI_BIG_JUMP_LEFT);
			}
			else
				RenderJumping(ani,MARIO_ANI_BIG_HOLD_JUMP_RIGHT,MARIO_ANI_BIG_HOLD_JUMP_LEFT,MARIO_ANI_BIG_HOLD_JUMP_RIGHT,MARIO_ANI_BIG_HOLD_JUMP_LEFT);
		}
		else
		{
			if (!isHolding)
			{
				int ani_go_right = MARIO_ANI_BIG_WALK_RIGHT;
				int ani_go_left = MARIO_ANI_BIG_WALK_LEFT;
				if (isRunning)
					if (abs(vx) >= MARIO_RUNNING_SPEED)
					{
						ani_go_right = MARIO_ANI_BIG_RUN_RIGHT;
						ani_go_left = MARIO_ANI_BIG_RUN_LEFT;
					}
				if (abs(vx) >= MARIO_RUNNING_MAXSPEED)
				{
					ani_go_right = MARIO_ANI_BIG_RUN_RIGHT_MAXSPEED;
					ani_go_left = MARIO_ANI_BIG_RUN_LEFT_MAXSPEED;
				}
				BasicRenderLogicsForAllLevel(ani,
					MARIO_ANI_BIG_IDLE_RIGHT, MARIO_ANI_BIG_IDLE_LEFT,
					MARIO_ANI_BIG_JUMP_RIGHT, MARIO_ANI_BIG_JUMP_LEFT,			
					MARIO_ANI_BIG_TURN_RIGHT, MARIO_ANI_BIG_TURN_LEFT,
					ani_go_right, ani_go_left, MARIO_ANI_BIG_KICK_RIGHT, MARIO_ANI_BIG_KICK_LEFT);
			}
			else
				BasicRenderLogicsForAllLevel(ani,
					MARIO_ANI_BIG_HOLD_IDLE_RIGHT, MARIO_ANI_BIG_HOLD_IDLE_LEFT,
					MARIO_ANI_BIG_HOLD_JUMP_RIGHT, MARIO_ANI_BIG_HOLD_JUMP_LEFT,
					MARIO_ANI_BIG_TURN_RIGHT, MARIO_ANI_BIG_TURN_LEFT,
					MARIO_ANI_BIG_HOLD_WALK_RIGHT, MARIO_ANI_BIG_HOLD_WALK_LEFT, MARIO_ANI_BIG_KICK_RIGHT, MARIO_ANI_BIG_KICK_LEFT);
		}
		if (state == MARIO_STATE_WORLD_MAP)
			ani = MARIO_ANI_BIG_WORLD_MAP;
	}
	else if (level == MARIO_LEVEL_RACOON)
	{
		if (state == MARIO_STATE_SIT)
		{
			if (nx > 0)
				ani = MARIO_ANI_RACOON_SIT_RIGHT;
			else
				ani = MARIO_ANI_RACOON_SIT_LEFT;
		}
		else if (isJumping)
		{
			if (!isHolding)
			{
				if (isFlying)
				{
					RenderJumping(ani,MARIO_ANI_RACOON_FLY_RIGHT,MARIO_ANI_RACOON_FLY_LEFT,MARIO_ANI_RACOON_FLY_RIGHT,MARIO_ANI_RACOON_FLY_LEFT);
				}
				else
					RenderJumping(ani,MARIO_ANI_RACOON_JUMP_RIGHT,MARIO_ANI_RACOON_JUMP_LEFT,MARIO_ANI_RACOON_JUMP_RIGHT,MARIO_ANI_RACOON_JUMP_LEFT);
			}
			else
				RenderJumping(ani,MARIO_ANI_RACOON_HOLD_JUMP_RIGHT,MARIO_ANI_RACOON_HOLD_JUMP_LEFT,MARIO_ANI_RACOON_HOLD_JUMP_RIGHT,MARIO_ANI_RACOON_HOLD_JUMP_LEFT);
			if (!isFlying)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACOON_FLOAT_RIGHT;
				if (nx < 0)
					ani = MARIO_ANI_RACOON_FLOAT_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_ANI_RACOON_FLY_RIGHT;
				if (nx < 0)
					ani = MARIO_ANI_RACOON_FLY_LEFT;
			}
		}
		else
		{
			if (!isHolding)
			{
				int ani_go_right = MARIO_ANI_RACOON_WALK_RIGHT;
				int ani_go_left = MARIO_ANI_RACOON_WALK_LEFT;
				if (isRunning)
					if (abs(vx) >= MARIO_RUNNING_SPEED)
					{
						ani_go_right = MARIO_ANI_RACOON_RUN_RIGHT;
						ani_go_left = MARIO_ANI_RACOON_RUN_LEFT;
					}
				if (abs(vx) >= MARIO_RUNNING_MAXSPEED)
				{
					ani_go_right = MARIO_ANI_RACOON_RUN_RIGHT_MAXSPEED;
					ani_go_left = MARIO_ANI_RACOON_RUN_LEFT_MAXSPEED;
				}
				BasicRenderLogicsForAllLevel(ani,
					MARIO_ANI_RACOON_IDLE_RIGHT, MARIO_ANI_RACOON_IDLE_LEFT,
					MARIO_ANI_RACOON_JUMP_RIGHT, MARIO_ANI_RACOON_JUMP_LEFT,
					MARIO_ANI_RACOON_TURN_RIGHT, MARIO_ANI_RACOON_TURN_LEFT,
					ani_go_right, ani_go_left, MARIO_ANI_RACOON_KICK_RIGHT, MARIO_ANI_RACOON_KICK_LEFT);
			}
			else
				BasicRenderLogicsForAllLevel(ani,
					MARIO_ANI_RACOON_HOLD_IDLE_RIGHT, MARIO_ANI_RACOON_HOLD_IDLE_LEFT,
					MARIO_ANI_RACOON_HOLD_JUMP_RIGHT, MARIO_ANI_RACOON_HOLD_JUMP_LEFT,
					MARIO_ANI_RACOON_TURN_RIGHT, MARIO_ANI_RACOON_TURN_LEFT,
					MARIO_ANI_RACOON_HOLD_WALK_RIGHT, MARIO_ANI_RACOON_HOLD_WALK_LEFT, MARIO_ANI_RACOON_KICK_RIGHT, MARIO_ANI_RACOON_KICK_LEFT);
		}
		if (isTurningTail)
		{
			if (nx > 0)
				ani = MARIO_ANI_RACOON_SPIN_RIGHT;
			else if (nx < 0)
				ani = MARIO_ANI_RACOON_SPIN_LEFT;
		}
		if (state == MARIO_STATE_WORLD_MAP)
			ani = MARIO_ANI_RACOON_WORLD_MAP;
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (state == MARIO_STATE_SIT)
		{
			if (nx > 0)
				ani = MARIO_ANI_FIRE_SIT_RIGHT;
			else
				ani = MARIO_ANI_FIRE_SIT_LEFT;
		}
		else if (isJumping)
		{
			if (!isHolding)
			{
				if (isFlying)
				{
					RenderJumping(ani,MARIO_ANI_FIRE_FLY_RIGHT,MARIO_ANI_FIRE_FLY_LEFT,MARIO_ANI_FIRE_FLY_RIGHT,MARIO_ANI_FIRE_FLY_LEFT);
				}
				else
					RenderJumping(ani,MARIO_ANI_FIRE_JUMP_RIGHT,MARIO_ANI_FIRE_JUMP_LEFT,MARIO_ANI_FIRE_JUMP_RIGHT,MARIO_ANI_FIRE_JUMP_LEFT);
			}
			else
				RenderJumping(ani,MARIO_ANI_FIRE_HOLD_JUMP_RIGHT,MARIO_ANI_FIRE_HOLD_JUMP_LEFT,MARIO_ANI_FIRE_HOLD_JUMP_RIGHT,MARIO_ANI_FIRE_HOLD_JUMP_LEFT);
		}
		else
		{
			if (!isHolding)
			{
				int ani_go_right = MARIO_ANI_FIRE_WALK_RIGHT;
				int ani_go_left = MARIO_ANI_FIRE_WALK_LEFT;
				if (isRunning)
					if (abs(vx) >= MARIO_RUNNING_SPEED)
					{
						ani_go_right = MARIO_ANI_FIRE_RUN_RIGHT;
						ani_go_left = MARIO_ANI_FIRE_RUN_LEFT;
					}
				if (abs(vx) >= MARIO_RUNNING_MAXSPEED)
				{
					ani_go_right = MARIO_ANI_FIRE_RUN_RIGHT_MAXSPEED;
					ani_go_left = MARIO_ANI_FIRE_RUN_LEFT_MAXSPEED;
				}
				BasicRenderLogicsForAllLevel(ani,
					MARIO_ANI_FIRE_IDLE_RIGHT, MARIO_ANI_FIRE_IDLE_LEFT,
					MARIO_ANI_FIRE_JUMP_RIGHT, MARIO_ANI_FIRE_JUMP_LEFT,
					MARIO_ANI_FIRE_TURN_RIGHT, MARIO_ANI_FIRE_TURN_LEFT,
					ani_go_right, ani_go_left, MARIO_ANI_FIRE_KICK_RIGHT, MARIO_ANI_FIRE_KICK_LEFT);
			}
			else
				BasicRenderLogicsForAllLevel(ani,
					MARIO_ANI_FIRE_HOLD_IDLE_RIGHT, MARIO_ANI_FIRE_HOLD_IDLE_LEFT,
					MARIO_ANI_FIRE_HOLD_JUMP_RIGHT, MARIO_ANI_FIRE_HOLD_JUMP_LEFT,
					MARIO_ANI_FIRE_TURN_RIGHT, MARIO_ANI_FIRE_TURN_LEFT,
					MARIO_ANI_FIRE_HOLD_WALK_RIGHT, MARIO_ANI_FIRE_HOLD_WALK_LEFT, MARIO_ANI_FIRE_KICK_RIGHT, MARIO_ANI_FIRE_KICK_LEFT);
		}
		if (state == MARIO_STATE_WORLD_MAP)
			ani = MARIO_ANI_FIRE_WORLD_MAP;
	}

	if (untouchable == 1) alpha = 120;
	animation_set->at(ani)->Render(x, y, alpha);
	for (size_t i = 0; i < listFire.size(); i++)
	{
		listFire[i]->Render();
	}
	for (size_t i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}
	//RenderBoundingBox();
}
void CMario::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_IDLE:
		if (vx > 0)
			ax = -MARIO_ACCELERATION;
		if (vx < 0)
			ax = MARIO_ACCELERATION;
		ay = MARIO_GRAVITY;
		if (abs(vx) <= MARIO_WALKING_SPEED)
		{
			vx = 0;
			ax = 0;
		}
		if (isRunning)
			StopRunning();
		break;
	case MARIO_STATE_WALK_RIGHT:
		if (ax < 0 && vy > 0)
		{
			isChangeDirection = true;
		}
		if (ax < 0 && RunningStacks != 0)
		{
			StopRunning();
			StartSlowDown();
		}
		ax = MARIO_ACCELERATION;
		nx = 1;
		break;
	case MARIO_STATE_WALK_LEFT:
		if (ax > 0 && vy > 0)
			isChangeDirection = true;
		if (ax > 0 && RunningStacks != 0)
		{
			StopRunning();
			StartSlowDown();
		}
		ax = -MARIO_ACCELERATION;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (vy > -MARIO_JUMP_SPEED_MIN)
			vy = -MARIO_JUMP_SPEED_MIN;
		if (RunningStacks == 7)
		{
			isFlying = true;
			StartFlying();
		}
		ay = -MARIO_ACCELERATION_JUMP;

		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_SIT:
		if (level != MARIO_LEVEL_SMALL)
		{
			if (abs(vx) <= MARIO_WALKING_SPEED) {
				vx = 0;
			}
			ay = MARIO_GRAVITY;
			ax = -nx * MARIO_ACCELERATION;
			isSitting = true;
		}
		break;
	case MARIO_STATE_TURN:
	case MARIO_STATE_RUN_RIGHT:
	case MARIO_STATE_RUN_LEFT:
	case MARIO_STATE_FLY_RIGHT: 
	case MARIO_STATE_FLY_LEFT:
	case MARIO_STATE_KICK:
	case MARIO_STATE_SHOOT_FIRE:
	case MARIO_STATE_SPIN:
	case MARIO_STATE_HOLD:
	case MARIO_STATE_RUN_MAXSPEED:
	case MARIO_STATE_WORLD_MAP:
		break;
	}
	if (state == MARIO_STATE_SIT && this->state != MARIO_STATE_SIT)
		y += MARIO_SIT_PUSH_BACK;
	if (state != MARIO_STATE_SIT && this->state == MARIO_STATE_SIT)
		y -= MARIO_SIT_PUSH_BACK;
	CGameObject::SetState(state);
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
		else if (isTurningTail){
			if (nx > 0) right = x + MARIO_RACOON_BBOX_SPIN_WIDTH + 3;
			else {
				left = x - 3;
				right = x + MARIO_RACOON_BBOX_SPIN_WIDTH;
			}
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