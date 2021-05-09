#include "Koopas.h"
#include "Game.h"
CKoopas::CKoopas(int &model, int &direction,CMario* mario)
{
	player = mario;
	isDefend = 0;
	nx = direction;
	SetState(KOOPAS_STATE_DEFEND);
}


void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGame *game = CGame::GetInstance();
	CGameObject::Update(dt);
	if (!isBeingHeld)
		vy += MARIO_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != KOOPAS_STATE_DIE || this->isBeingHeld)
		CalcPotentialCollisions(coObjects, coEvents);
	
	if (isBeingHeld) {	//-----------------Hold Turtle-------------------------//
		if (player->GetLevel() == MARIO_LEVEL_SMALL)
		{
			if (player->nx > 0)
				SetPosition(player->x + 10, player->y - 2 );
			else
				SetPosition(player->x - 14, player->y - 2);
		}
		else if (player->GetLevel() == MARIO_LEVEL_RACOON)
		{
			if (player->nx > 0)
				SetPosition(player->x + 11, player->y + 6);
			else
				SetPosition(player->x - 14, player->y + 6);
		}
		else if (player->GetLevel() == MARIO_LEVEL_BIG || player->GetLevel() == MARIO_LEVEL_FIRE)
		{
			if (player->nx > 0)
				SetPosition(player->x + 11, player->y + 6);
			else
				SetPosition(player->x - 14, player->y + 6);
		}

	}

	if (state == KOOPAS_STATE_WALKING){
		if (nx > 0 && isDefend == 0)
			vx = KOOPAS_WALKING_SPEED * dt;
		if (nx < 0 && isDefend == 0)
			vx = -KOOPAS_WALKING_SPEED * dt;
	}
	if (GetTickCount() - defend_start > KOOPAS_DEFEND_TIME && isDefend == 1)
	{
		defend_start = 0;
		isDefend = 0;		
		y -= 16;
		isBeingHeld = false;
		state = KOOPAS_STATE_WALKING;
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			/*if (dynamic_cast<CMario *>(e->obj)) {
				player->vy = -MARIO_JUMP_DEFLECT_SPEED;
				CMario *player = dynamic_cast<CMario *>(e->obj);
				if (e->ny > 0)
				{
					if (state == KOOPAS_STATE_DEFEND) {
						DebugOut(L"ok");
						player->vy = -MARIO_JUMP_DEFLECT_SPEED;
						state = KOOPAS_STATE_BALL;
					}
					else if (state != KOOPAS_STATE_DIE)
					{
						state = KOOPAS_STATE_DEFEND;
						player->vy = -MARIO_JUMP_DEFLECT_SPEED;
					}

				}
				else if (e->nx != 0)
				{
					if (game->IsKeyDown(DIK_Q) && state == KOOPAS_STATE_DEFEND) {
						player->isHolding = true;
						isBeingHeld = true;
					}
					else if (state == KOOPAS_STATE_DEFEND)
					{
						player->SetState(MARIO_STATE_KICK);
						if (nx > 0) nx = -1;
						else nx = 1;
						state = KOOPAS_STATE_BALL;
					}
					else if (player->state == MARIO_STATE_SPIN && player->level == MARIO_LEVEL_RACOON) {
						nx = this->nx;
						state = KOOPAS_STATE_DIE;
					}
					else if (player->untouchable == 0)
					{
						if (state != KOOPAS_STATE_DIE)
						{
							if (player->level > MARIO_LEVEL_SMALL)
							{
								player->level = MARIO_LEVEL_SMALL;
								player->StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}*/
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = -1;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (state == KOOPAS_STATE_WALKING) {
		if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
		else ani = KOOPAS_ANI_WALKING_LEFT;
	}
	else if (state == KOOPAS_STATE_DEFEND) {
		ani = KOOPAS_ANI_DEFEND;
	}
		
	else if (state == KOOPAS_STATE_BALL) {
		ani = KOOPAS_ANI_BALL;
	}
	if (state == KOOPAS_STATE_REVIVE) {
	}

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		//y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		if (nx > 0)
			vx = FIRE_SPEED*dt;
		else vx = -FIRE_SPEED*dt;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case KOOPAS_STATE_WALKING:
		if ( nx >0 && isDefend == 0)
			vx = KOOPAS_WALKING_SPEED *dt ;
		if (nx < 0 && isDefend == 0)
			vx = -KOOPAS_WALKING_SPEED *dt;
		break;
	case KOOPAS_STATE_DEFEND:
		vx = 0;
		StartDefendTime();
		break;
	case KOOPAS_STATE_BALL:
		if (nx>0)
			vx = KOOPAS_BALL_SPEED * dt;
		else vx = -KOOPAS_BALL_SPEED * dt;
		break;
	}

}
void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (state == KOOPAS_STATE_DEFEND || state == KOOPAS_STATE_BALL)
	{
		right = left + KOOPAS_DEFEND_HITBOX;
		bottom = top + KOOPAS_DEFEND_HITBOX;
	}
	else
	{
		right = x + KOOPAS_WALK_HITBOX_WIDTH;
		bottom = y + KOOPAS_WALK_HITBOX_HEIGHT;
	}
}