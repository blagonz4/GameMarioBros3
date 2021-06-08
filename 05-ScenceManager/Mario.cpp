#include"Mario.h"
#include "Game.h"
#include "FireBall.h"
#include "Goomba.h"
#include "Platform.h"
#include "ColorBlock.h"
#include "QuestionBrick.h"
#include "GoldBrick.h"
#include "Coin.h"
#include "PSwitch.h"
#include "Leaf.h"
#include "Mushroom.h"
#include "EffectPoint.h"
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

	if (vx * nx < 0) {
		SetState(MARIO_STATE_TURN);
	}
	// Simple fall down
	vy += MARIO_GRAVITY * dt;
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
		if (!CheckObjectInCamera(listFire.at(i)) || listFire.at(i)->isFinish == true) {
			listFire.erase(listFire.begin() + i);
		}
	}
	for (size_t i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Update(dt, coObjects);
		if (!CheckObjectInCamera(listEffect.at(i)) || listEffect.at(i)->isFinish == true) {
			listEffect.erase(listEffect.begin() + i);
		}
	}
	//------------------------------------------------------------------
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount() - limitfly_start > MARIO_LIMIT_FLY_TIME)
	{
		isFlying = false;
		limitfly_start = 0;
	}
	if (GetTickCount() - limitjump_start > MARIO_LIMIT_JUMP_TIME)
	{
		isJumping = false;
		limitjump_start = 0;
	}

	if (isTransformToBig) {
		timeTransform += dt;
		if (timeTransform < 200) {
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
								ShowEffectPoint(goomba, POINT_EFFECT_MODEL_100);
							}
							else 
							{
								goomba->SetState(GOOMBA_STATE_DIE);
							}
							this->vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}				
				}
				else if (e->nx != 0 || e->ny >0)
				{
					
					if (state == MARIO_STATE_SPIN && level == MARIO_LEVEL_RACOON) {
						goomba->nx = this->nx;
						goomba->SetState(GOOMBA_STATE_DIE);
					}
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
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
				if (e->ny < 0)
				{
					if (koopa->model == KOOPAS_MODEL_GREEN_WING) {
						koopa->model = 1;
						ShowEffectPoint(koopa, POINT_EFFECT_MODEL_100);
						koopa->SetState(KOOPAS_STATE_WALKING);
					}						
					if (koopa->GetState() == KOOPAS_STATE_DEFEND) {
						ShowEffectPoint(koopa, POINT_EFFECT_MODEL_100);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						koopa->SetState(KOOPAS_STATE_BALL);
					}
					else if (koopa->GetState() != KOOPAS_STATE_DIE)
					{
						ShowEffectPoint(koopa, POINT_EFFECT_MODEL_100);
						koopa->SetState(KOOPAS_STATE_DEFEND);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}	
				}
				else if (nx != 0)
				{
					if (game->IsKeyDown(DIK_Q) && koopa->state == KOOPAS_STATE_DEFEND) {
						this->isHolding = true;
						koopa->isBeingHeld = true;
					}
					else if (koopa->GetState() == KOOPAS_STATE_DEFEND)
					{
						this->SetState(MARIO_STATE_KICK);
						if (nx > 0) koopa->nx = -1;
						else koopa->nx = 1;
						koopa->SetState(KOOPAS_STATE_BALL);
					}
					else if (state == MARIO_STATE_SPIN && level == MARIO_LEVEL_RACOON) {
						koopa->nx = this->nx;
						koopa->SetState(KOOPAS_STATE_DIE);
					}				
					else if (untouchable == 0)
					{
						if (koopa->GetState() != KOOPAS_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
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
			else if (e->obj->GetType() == FIRE){ // if e->obj is fireball 
				if (untouchable == 0)
				{					
					if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			else if (e->obj->GetType() == FIREPLANT) { // if e->obj is fireball 
				if (untouchable == 0)
				{

					if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			else if (e->obj->GetType() == QUESTIONBRICK) { // if e->obj is fireball 
				QuestionBrick* qb = dynamic_cast<QuestionBrick*>(e->obj);
				if (e->ny > 0)
				{
					if (qb->Health == 1)
					{
						qb->vy = -QUESTION_BRICK_SPEED_UP *dt;
						qb->Health = 0;
						qb->isUnbox = true;						
					}
				}
			}
			else if (e->obj->GetType() == GOLDBRICK) { // if e->obj is fireball 
				GoldBrick* gb = dynamic_cast<GoldBrick*>(e->obj);
				int model = gb->model;
				//if (e->nx != 0) {
				//	DebugOut(L"quay duoi trung\n");
				//	gb->isFinish = true;
				//}
				if (e->ny > 0)
				{
					switch (model) {
					case GOLD_BRICK_MODEL_COIN:
						if (gb->y >= gb->minY) {
							gb->vy = -QUESTION_BRICK_SPEED_UP * dt;
						}
							
						//gb->SetState(GOLD_BRICK_STATE_IDLE_COIN);
						break;
					case GOLD_BRICK_MODEL_PSWITCH:
						gb->SetState(GOLD_BRICK_STATE_UNBOX);
						//gb->Health--;
						break;
					}
				}			
			}
			else if (e->obj->GetType() == COIN) { 
				//Coin* coin = dynamic_cast<Coin*>(e->obj);
				e->obj->isFinish = true;
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
			else if (e->obj->GetType() == MUSHROOM_POWER)
			{
				Mushroom* mushroom = dynamic_cast<Mushroom*>(e->obj);
				mushroom->isFinish = true;
				ShowEffectPoint(this, POINT_EFFECT_MODEL_1K);
				this->y -= 20;
				this->SetLevel(MARIO_LEVEL_BIG);			
			}
			else if (e->obj->GetType() == LEAF)
			{
				Leaf* leaf = dynamic_cast<Leaf*>(e->obj);
				leaf->isFinish = true; this->y -= 5;
				ShowEffectPoint(this, POINT_EFFECT_MODEL_1K);
				this->isTransformToRacoon = true;
				this->SetLevel(MARIO_LEVEL_RACOON);
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

			if (state == MARIO_STATE_RUN_RIGHT)
				ani = MARIO_ANI_BIG_RUN_RIGHT;
			if (state == MARIO_STATE_RUN_LEFT)
				ani = MARIO_ANI_BIG_RUN_LEFT;

			if (state == MARIO_STATE_RUN_MAXSPEED) {
				if (nx > 0)
					ani = MARIO_ANI_BIG_RUN_RIGHT_MAXSPEED;
				else ani = MARIO_ANI_BIG_RUN_LEFT_MAXSPEED;
			}

			if (state == MARIO_STATE_FLY_RIGHT)
				ani = MARIO_ANI_BIG_FLY_RIGHT;
			if (state == MARIO_STATE_FLY_LEFT)
				ani = MARIO_ANI_BIG_FLY_LEFT;

			if (state == MARIO_STATE_JUMP && isJumping) {
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
		}
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
		}
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
				if (nx > 0) ani = MARIO_ANI_RACOON_FLOAT_RIGHT;
				else ani = MARIO_ANI_RACOON_FLOAT_LEFT;
			}

			if (state == MARIO_STATE_SIT) {
				if (nx > 0) ani = MARIO_ANI_RACOON_SIT_RIGHT;
				else ani = MARIO_ANI_RACOON_SIT_LEFT;
			}

			if (state == MARIO_STATE_KICK) {
				if (nx > 0) ani = MARIO_ANI_RACOON_KICK_RIGHT;
				else ani = MARIO_ANI_RACOON_KICK_LEFT;
			}

			if (state == MARIO_STATE_SPIN) {
				if (nx > 0) ani = MARIO_ANI_RACOON_SPIN_RIGHT;
				else ani = MARIO_ANI_RACOON_SPIN_LEFT;
			}

		}
		int alpha = 255;
		if (untouchable) alpha = 128;
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
		else if (state == MARIO_STATE_SPIN){
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
	listEffect.push_back(effectPoint);
}


