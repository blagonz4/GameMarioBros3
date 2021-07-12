#include "Koopas.h"
#include "Platform.h"
#include "Pipe.h"
#include "QuestionBrick.h"
#include "GoldBrick.h"
CKoopas::CKoopas(float &model, float &direction,CMario* mario)
{
	this->model = model;
	if (this->model == KOOPAS_MODEL_RED || this->model == KOOPAS_MODEL_GREEN)
		Health = 1;
	else Health = 2;
	player = mario;
	isDefend = 0;
	nx = direction;

	if (Health == 2) SetState(KOOPAS_STATE_FLY);
	else SetState(KOOPAS_STATE_WALKING);

	eType = Type::KOOPAS;
	objType = ObjectType::ENEMY;
}


void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (!isBeingHeld)
		vy += MARIO_GRAVITY * dt;
	if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_FLY) {
		if (nx > 0)
			vx = KOOPAS_WALKING_SPEED * dt;
		else
			vx = -KOOPAS_WALKING_SPEED * dt;
	}

	if (state == KOOPAS_STATE_FLY) {
		vy -= KOOPAS_FLY_SPEED;
	}

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
	
	if (!player->isHolding && isBeingHeld) {
		isBeingHeld = false;
		this->nx = player->nx;
		SetState(KOOPAS_STATE_BALL);
	}

	if (GetTickCount() - defend_start > KOOPAS_DEFEND_TIME && isDefend == 1)
	{
		defend_start = 0;
		isDefend = 0;		
		y -= 16;
		isBeingHeld = false;
		SetState(KOOPAS_STATE_WALKING);
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
		if (ny != 0 && state == KOOPAS_STATE_FLY) vy = -KOOPAS_FLY_SPEED * dt;
		else if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == COLORBLOCK)
			{
				if (e->nx != 0) {
					this->nx *= -1;
					if (this->state == KOOPAS_STATE_BALL) {
						this->vx = this->nx * KOOPAS_BALL_SPEED * dt;
					}
					else if (this->state == KOOPAS_STATE_WALKING) {
						this->vx = this->nx * KOOPAS_WALKING_SPEED * dt;
					}
				}
			}

			else if (e->obj->GetType() == PIPE)
			{
				if (e->nx != 0) {
					this->nx *= -1;
					if (this->state == KOOPAS_STATE_BALL) {
							this->vx = this->nx * KOOPAS_BALL_SPEED * dt;
					}
					else if (this->state == KOOPAS_STATE_WALKING) {
							this->vx = this->nx * KOOPAS_WALKING_SPEED * dt;
					}
				}				
			}

			else if (e->obj->GetType() == PLATFORM) {
				if (e->nx != 0) {
					this->nx *= -1;
					if (this->state == KOOPAS_STATE_BALL) {
						this->vx = this->nx * KOOPAS_BALL_SPEED * dt;
					}
					else if (this->state == KOOPAS_STATE_WALKING) {
						this->vx = this->nx * KOOPAS_WALKING_SPEED * dt;
					}
				}
			}
			else if (e->obj->GetType() == QUESTIONBRICK) {
				QuestionBrick* qb = dynamic_cast<QuestionBrick*>(e->obj);
				if (e->nx != 0) {
					this->nx *= -1;
					if (this->state == KOOPAS_STATE_BALL) {
						this->vx = this->nx * KOOPAS_BALL_SPEED * dt;
						if (qb->Health == 1)
						{
							qb->vy = -QUESTION_BRICK_SPEED_UP * dt;
							qb->Health = 0;
						}
					}
					else if (this->state == KOOPAS_STATE_WALKING) {
						this->vx = this->nx * KOOPAS_WALKING_SPEED * dt;
					}
				}
			}
			else if (e->obj->GetType() == GOLDBRICK) {
				GoldBrick* gb = dynamic_cast<GoldBrick*>(e->obj);
				if (e->nx != 0) {
					this->nx *= -1;
					if (state == KOOPAS_STATE_WALKING)
						this->vx = this->nx * KOOPAS_WALKING_SPEED *dt ;
					if (state == KOOPAS_STATE_BALL) {
						this->vx = this->nx * KOOPAS_BALL_SPEED *dt ;
						int model = (int)gb->model;
						switch (model)
						{
						case GOLD_BRICK_MODEL_COIN:
							//gb->SetState(GOLD_BRICK_STATE_IDLE_COIN);
							gb->isFinish = true;
							break;
						case GOLD_BRICK_MODEL_PSWITCH:
							gb->SetState(GOLD_BRICK_STATE_UNBOX);
							break;
						}
					}
				}								
			}
			else if (e->obj->GetObjectType() == ENEMY) {
				x += dx;
				if (this->GetState() == KOOPAS_STATE_BALL) {
					this->vx = this->nx * KOOPAS_BALL_SPEED * dt;
					e->obj->isFinish = true;
				}		
			}
			else if (e->obj->GetType() == MUSHROOM_1_UP || e->obj->GetType() == MUSHROOM_POWER) {
				x += dx;
				this->vx = this->nx * KOOPAS_BALL_SPEED * dt;
			}
			else if (e->obj->GetType() == COIN) {
				x += dx;
				this->vx = this->nx * KOOPAS_WALKING_SPEED * dt;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = -1;
	if (model == KOOPAS_MODEL_RED) {
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_ANI_RED_DIE;
		}
		else if (state == KOOPAS_STATE_WALKING) {
			if (vx > 0) ani = KOOPAS_ANI_RED_WALKING_RIGHT;
			else ani = KOOPAS_ANI_RED_WALKING_LEFT;
		}
		else if (state == KOOPAS_STATE_DEFEND) {
			ani = KOOPAS_ANI_RED_DEFEND;
		}

		else if (state == KOOPAS_STATE_BALL) {
			ani = KOOPAS_ANI_RED_BALL;
		}

	}

	else {
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_ANI_GREEN_DIE;
		}
		else if (state == KOOPAS_STATE_WALKING) {
			if (vx > 0) ani = KOOPAS_ANI_GREEN_WALKING_RIGHT;
			else ani = KOOPAS_ANI_GREEN_WALKING_LEFT;
		}
		else if (state == KOOPAS_STATE_DEFEND) {
			ani = KOOPAS_ANI_GREEN_DEFEND;
		}

		else if (state == KOOPAS_STATE_BALL) {
			ani = KOOPAS_ANI_GREEN_BALL;
		}
		else if (state == KOOPAS_STATE_FLY) {
			if (vx > 0) ani = KOOPAS_ANI_GREEN_FLY_RIGHT;
			else ani = KOOPAS_ANI_GREEN_FLY_LEFT;
		}
	}
	
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vy = MARIO_DIE_DEFLECT_SPEED*dt;
		break;
	case KOOPAS_STATE_WALKING:
		
		break;
	case KOOPAS_STATE_DEFEND:
		vx = 0;
		StartDefendTime();
		break;
	case KOOPAS_STATE_BALL:
		vx = nx * KOOPAS_BALL_SPEED * dt; 
		StartDefendTime();
		break;
	case KOOPAS_STATE_FLY:
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