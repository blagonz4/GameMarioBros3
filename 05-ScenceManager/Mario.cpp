#include"Mario.h"
#include "Game.h"
#include "FireBall.h"
#include "Goomba.h"
#include "Platform.h"
CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGame *game = CGame::GetInstance();
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY*dt;
	DebugOut(L"vy: %d \n", vy);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	
	//-----------------------------FIRE BALL-------------------------------
	
	if (state == MARIO_STATE_SHOOT_FIRE) {
		if (listFire.size() < 2) {
			FireBall *fire = new FireBall(x + 5, y + 5);
			fire->nx = nx;
			DebugOut(L"nx cua Fire: %d \n", fire->nx);
			listFire.push_back(fire);
		}
		
	}
	for (int i = 0; i < listFire.size(); i++)
	{
		listFire[i]->Update(dt, coObjects);
		if (!CheckObjectInCamera(listFire.at(i)) || listFire.at(i)->isFinish == true) {
			listFire.erase(listFire.begin() +i);
		}
	}


	//------------------------------------------------------------------
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount() - limitjump_start > MARIO_LMIT_JUMP_TIME)
	{
		isOnAir = 0;
		limitjump_start = 0;
	}
	// No collision occured, proceed normally
	if (coEvents.size()==0)
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					
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
			else if (dynamic_cast<CKoopas *>(e->obj))
			{
				CKoopas *koopa = dynamic_cast<CKoopas *>(e->obj);
				if (e->ny < 0)
				{
					if (koopa->GetState() == KOOPAS_STATE_DEFEND) {
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						koopa->SetState(KOOPAS_STATE_BALL);
					}
					else if (koopa->GetState() != KOOPAS_STATE_DIE)
					{
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
			//else if (dynamic_cast<ColorBlock *>(e->obj))
			//{
			//	ColorBlock *block = dynamic_cast<ColorBlock *>(e->obj);

			//	if (e->ny > 0) {
			//		//SetState(MARIO_STATE_GET_THROUGH);
			//		//block->SetState(COLOR_BLOCK_GET_THROUGH);
			//	}
			//}
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
			//if (state == MARIO_STATE_TURN)
			//{
			//	if (nx < 0)
			//		ani = MARIO_ANI_BIG_TURN_LEFT;

			//	if (nx > 0)
			//		ani = MARIO_ANI_BIG_TURN_RIGHT;
			//}

			//if (state == MARIO_STATE_WALK_RIGHT)
			//	ani = MARIO_ANI_BIG_WALK_RIGHT;
			//if (state == MARIO_STATE_WALK_LEFT)
			//	ani = MARIO_ANI_BIG_WALK_LEFT;


			if (state == MARIO_STATE_RUN_RIGHT)
				ani = MARIO_ANI_BIG_RUN_RIGHT;
			if (state == MARIO_STATE_RUN_LEFT)
				ani = MARIO_ANI_BIG_RUN_LEFT;

			if (abs(vx) == MARIO_RUNNING_MAXSPEED)
				if (nx > 0)
					ani = MARIO_ANI_BIG_RUN_RIGHT_MAXSPEED;
				else ani = MARIO_ANI_BIG_RUN_LEFT_MAXSPEED;

			if (state == MARIO_STATE_FLY_RIGHT)
				ani = MARIO_ANI_BIG_FLY_RIGHT;
			if (state == MARIO_STATE_FLY_LEFT)
				ani = MARIO_ANI_BIG_FLY_LEFT;


			if (state == MARIO_STATE_JUMP) {
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

			if (state == MARIO_STATE_FLY_RIGHT)
				ani = MARIO_ANI_SMALL_FLY_RIGHT;
			if (state == MARIO_STATE_FLY_LEFT)
				ani = MARIO_ANI_SMALL_FLY_LEFT;

			if (state == MARIO_STATE_JUMP) {
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

			if (abs(vx) == MARIO_RUNNING_MAXSPEED)
				if (nx > 0)
					ani = MARIO_ANI_FIRE_RUN_RIGHT_MAXSPEED;
				else ani = MARIO_ANI_FIRE_RUN_LEFT_MAXSPEED;

			if (state == MARIO_STATE_FLY_RIGHT)
				ani = MARIO_ANI_FIRE_FLY_RIGHT;
			if (state == MARIO_STATE_FLY_LEFT)
				ani = MARIO_ANI_FIRE_FLY_LEFT;


			if (state == MARIO_STATE_JUMP) {
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

			if (abs(vx) == MARIO_RUNNING_MAXSPEED)
				if (nx > 0)
					ani = MARIO_ANI_RACOON_RUN_RIGHT_MAXSPEED;
				else ani = MARIO_ANI_RACOON_RUN_LEFT_MAXSPEED;

			if (state == MARIO_STATE_FLY_RIGHT)
				ani = MARIO_ANI_RACOON_FLY_RIGHT;
			if (state == MARIO_STATE_FLY_LEFT)
				ani = MARIO_ANI_RACOON_FLY_LEFT;


			if (state == MARIO_STATE_JUMP) {
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

			if (state == MARIO_STATE_SPIN) {
				if (nx > 0) ani = MARIO_ANI_RACOON_SPIN_RIGHT;
				else ani = MARIO_ANI_RACOON_SPIN_LEFT;
			}

		}
		int alpha = 255;
		if (untouchable) alpha = 128;
		animation_set->at(ani)->Render(x, y, alpha);

		for (int i = 0; i < listFire.size(); i++)
		{
			listFire[i]->Render();
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
		vx = MARIO_WALKING_SPEED*dt;
		break;
	case MARIO_STATE_WALK_LEFT:
		vx = -MARIO_WALKING_SPEED*dt;
		break;
	case MARIO_STATE_JUMP:
		vy = -MARIO_JUMP_SPEED * dt;
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
		vx = MARIO_RUNNING_SPEED * dt;
		nx = 1;
		break;
	case MARIO_STATE_RUN_LEFT:
		vx = -MARIO_RUNNING_SPEED * dt;
		nx = -1;
		break;
	case MARIO_STATE_FLY_RIGHT:
		vx = MARIO_RUNNING_MAXSPEED * dt;
		vy = -MARIO_JUMP_SPEED * dt;
		nx = 1;
		break;
	case MARIO_STATE_FLY_LEFT:
		vx = -MARIO_RUNNING_MAXSPEED * dt;
		vy = -MARIO_JUMP_SPEED * dt;
		nx = -1;
		break;
	case MARIO_STATE_KICK:
	case MARIO_STATE_SHOOT_FIRE:
	case MARIO_STATE_SPIN:
	case MARIO_STATE_HOLD:
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

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}




