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

	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL && mario->level == MARIO_LEVEL_RACOON && mario->isTurningTail) {
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (CheckAABB(mLeft, mTop + TAIL_SIZE, mRight, mBottom)) {
			this->vy = -KOOPAS_DEFLECT_SPEED;
			this->vx = mario->nx * KOOPAS_DEFLECT_SPEED;
			SetState(KOOPAS_STATE_UP_SIDE_DOWN);			
		}
	}

	if (!isBeingHeld)
		vy += MARIO_GRAVITY * dt;
	if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_FLY) {
		if (nx > 0)
			vx = KOOPAS_WALKING_SPEED;
		else
			vx = -KOOPAS_WALKING_SPEED;
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
		mario->StartKicking();
		SetState(KOOPAS_STATE_BALL);
	}

	if (GetTickCount() - defend_start > KOOPAS_DEFEND_TIME && isDefend == 1)
	{
		defend_start = 0;
		isDefend = 0;		
		y -= KOOPAS_TURN_BACK_SIZE;
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
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x0 = x; y0 = y;
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			GetBoundingBox(mLeft, mTop, mRight, mBottom);

			if (e->obj->GetType() == COLORBLOCK)
			{
				if (e->ny < 0)
				{
					vy = 0;
					if (state == KOOPAS_STATE_DEFEND)
						vx = 0;
					if (state == KOOPAS_STATE_WALKING)
					{
						e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
						if (this->nx > 0 && x >= oRight - 10.f)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = -1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
						if (this->nx < 0 && x <= e->obj->x - 10.f)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = 1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
					}
					if (model == KOOPAS_MODEL_GREEN_WING)
					{
						y = e->obj->y - KOOPAS_BBOX_HEIGHT;
						vy = -KOOPAS_FLY_SPEED;
					}
				}
				else
				{
					if (e->nx != 0) {
						x = x0 + dx;
					}						
					if (state == KOOPAS_STATE_DEFEND && e->ny > 0)
						y = y0 + dy;
				}
			}
			else if (e->obj->GetType() == PIPE)
			{
				if (ny != 0) {
					vy = 0;
				} 
				if (e->nx != 0) {
					this->nx *= -1;
					if (this->state == KOOPAS_STATE_BALL) {
						this->vx = this->nx * KOOPAS_BALL_SPEED;
					}
					else if (this->state == KOOPAS_STATE_WALKING) {
						this->vx = this->nx * KOOPAS_WALKING_SPEED;
					}
				}
			}
			else if (e->obj->GetType() == PLATFORM ) {
				vy = 0;
				y = y0;		
				if (model == KOOPAS_MODEL_GREEN_WING)
				{
					y = e->obj->y - KOOPAS_BBOX_HEIGHT;
					vy = -KOOPAS_FLY_SPEED;
				}
				if (e->nx != 0) {
					this->nx *= -1;
					this->vx = -this->vx;
				}
			}
			else if (e->obj->GetType() == MUSICBRICK) {
				MusicBrick* mb = dynamic_cast<MusicBrick*>(e->obj);
				if (e->ny != 0) {
					if (mb->isHidden) {
						x += dx; y += dy;
					}
					else {
						x = x0;
						vy = 0;
					}
				}
				if (e->nx != 0) {
					if (mb->isHidden) {
						x += dx; y += dy;
					}
					else {
						this->nx *= -1;
						if (this->state == KOOPAS_STATE_BALL) {
							this->vx = this->nx * KOOPAS_BALL_SPEED;
						}
						else if (this->state == KOOPAS_STATE_WALKING) {
							this->vx = this->nx * KOOPAS_WALKING_SPEED;
						}
					}
				}
			}
			else if (e->obj->GetType() == QUESTIONBRICK) {
				QuestionBrick* qb = dynamic_cast<QuestionBrick*>(e->obj);
				if (e->nx != 0) {
					this->nx *= -1;
					if (this->state == KOOPAS_STATE_BALL) {
						this->vx = this->nx * KOOPAS_BALL_SPEED;
						if (qb->Health == 1)
						{
							qb->vy = -QUESTION_BRICK_SPEED_UP;
							qb->Health = 0;
							qb->isUnbox = true;
						}
					}
					else if (this->state == KOOPAS_STATE_WALKING) {
						this->vx = this->nx * KOOPAS_WALKING_SPEED;
					}
				}
				if (e->ny != 0) vy = 0;
			}
			else if (e->obj->GetType() == GOLDBRICK) {
				GoldBrick* gb = dynamic_cast<GoldBrick*>(e->obj);
				GetBoundingBox(mLeft, mTop, mRight, mBottom);
				e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
				if (e->ny < 0)
				{
					vy = 0;
					if (state == KOOPAS_STATE_DEFEND)
						vx = 0;
					if (state == KOOPAS_STATE_WALKING)
					{
						if (this->nx > 0 && x >= oRight - 5.f)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = -1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
						if (this->nx < 0 && x <= e->obj->x - 5.f)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = 1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
					}
					if (model == KOOPAS_MODEL_GREEN_WING)
					{
						y = e->obj->y - KOOPAS_BBOX_HEIGHT;
						vy = -KOOPAS_FLY_SPEED;
					}
				}
				if (e->nx != 0 && mBottom + 0.4f > oTop) {
					this->nx *= -1;
					if (state == KOOPAS_STATE_WALKING)
						this->vx = this->nx * KOOPAS_WALKING_SPEED;
					if (state == KOOPAS_STATE_BALL) {
						this->vx = this->nx * KOOPAS_BALL_SPEED;
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
					this->vx = this->nx * KOOPAS_BALL_SPEED;
					e->obj->isFinish = true;
				}
			}
			else if (e->obj->GetType() == MUSHROOM_1_UP || e->obj->GetType() == MUSHROOM_POWER) {
				x += dx;
				this->vx = this->nx * KOOPAS_BALL_SPEED;
			}
			else if (e->obj->GetType() == COIN) {
				x += dx;
				this->vx = this->nx * KOOPAS_WALKING_SPEED;
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
		else if (state == KOOPAS_STATE_UP_SIDE_DOWN) {
			ani = KOOPAS_ANI_RED_DIE;
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
		else if (state == KOOPAS_STATE_UP_SIDE_DOWN) {
			ani = KOOPAS_ANI_GREEN_DIE;
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

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vy = MARIO_DIE_DEFLECT_SPEED;
		break;
	case KOOPAS_STATE_WALKING:
		break;
	case KOOPAS_STATE_DEFEND:
		vx = 0;
		StartDefendTime();
		break;
	case KOOPAS_STATE_BALL:
		vx = nx * KOOPAS_BALL_SPEED; 
		StartDefendTime();
		break;
	case KOOPAS_STATE_FLY:
		break;
	case KOOPAS_STATE_UP_SIDE_DOWN:
		this->vx = 0;
		StartDefendTime();
		break;
	}

}
void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (state == KOOPAS_STATE_DEFEND || state == KOOPAS_STATE_BALL || state == KOOPAS_STATE_UP_SIDE_DOWN)
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
bool CKoopas::CalTurnable(LPGAMEOBJECT object, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CheckObjectInCamera())
		return false;
	for (UINT i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<ColorBlock*>(coObjects->at(i)) || dynamic_cast<GoldBrick*>(coObjects->at(i)))
			if (abs(coObjects->at(i)->y == object->y))
			{
				if (nx > 0)
					if (coObjects->at(i)->x > object->x && coObjects->at(i)->x - BRICK_BBOX_WIDTH < object->x + BRICK_BBOX_WIDTH)
						return false;
				if (nx < 0)
					if (coObjects->at(i)->x + BRICK_BBOX_WIDTH > object->x - BRICK_BBOX_WIDTH && coObjects->at(i)->x < object->x)
						return false;
			}
	return true;
}