#include "FirePlant.h"



FirePlant::FirePlant(float x, float y,CMario* mario,float model)
{
	this->x = x; this->y = y;
	this->mario = mario;	
	this->marioRange = marioRange;

	if (model == BITE_PLANT) {	// PiranhaPlant
		this->limitY = this->y - PIRANHAPLANT_BBOX_HEIGHT;
	}	
	if (model == FIRE_PLANT_GREEN) {//GREEN
		plantHeight = FIRE_PLANT_GREEN_BBOX_HEIGHT;
		this->limitY = this->y - FIRE_PLANT_GREEN_BBOX_HEIGHT;
	}
	else {//RED
		plantHeight = FIRE_PLANT_RED_BBOX_HEIGHT;
		this->limitY = this->y - FIRE_PLANT_RED_BBOX_HEIGHT;
	}
	SetState(PIRANHAPLANT_STATE_INACTIVE);
	eType = Type::FIREPLANT;
	this->model = model;
}
void FirePlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL){	
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		int mWidth = (int)mRight - (int)mLeft;
		if ((floor(mario->x) + (float)mWidth + PIRANHAPLANT_ACTIVE_RANGE <= x
			|| ceil(mario->x) >= x + PIRANHAPLANT_BBOX_WIDTH + PIRANHAPLANT_ACTIVE_RANGE)
			&& state == PIRANHAPLANT_STATE_INACTIVE && ( biting_start == 0 || delay_start == 0))
			SetState(PIRANHAPLANT_STATE_DARTING);

		if (mario->level == MARIO_LEVEL_RACOON && mario->isTurningTail) {
			if (CheckAABB(mLeft, mTop + TAIL_SIZE, mRight, mBottom))
				isFinish = true;
		}
	}
	//BITE_PLANT
	if (model == BITE_PLANT) {
		if (y <= limitY && vy < 0)
		{
			y = limitY;
			SetState(PIRANHAPLANT_STATE_BITING);
		}
		if (y >= limitY + PIRANHAPLANT_BBOX_HEIGHT && vy > 0)
		{
			y = limitY + PIRANHAPLANT_BBOX_HEIGHT;
			SetState(PIRANHAPLANT_STATE_INACTIVE);
		}
		if (GetTickCount() - biting_start >= PIRANHAPLANT_BITING_TIME && biting_start != 0)
		{
			if (y == limitY)
				vy = PIRANHAPLANT_DARTING_SPEED;
			biting_start = 0;
		}
	}
	//FIRE PLANT
	else {
		if (y <= limitY && vy < 0)
		{
			y = limitY;
			vy = 0;
			StartAim();
		}
		if (y >= limitY + plantHeight && vy > 0)
		{
			y = limitY + plantHeight;
			SetState(PIRANHAPLANT_STATE_INACTIVE);
			StartDelay();
		}
		if (GetTickCount() - aim_start >= PIRANHAPLANT_AIM_TIME && aim_start != 0)
		{
			aim_start = 0;
			SetState(PIRANHAPLANT_STATE_SHOOTING);
			StartDelay();
		}
		if (GetTickCount() - delay_start >= PIRANHAPLANT_DELAY_TIME && delay_start != 0)
		{
			delay_start = 0;
			if (y == limitY)
				vy = PIRANHAPLANT_DARTING_SPEED;
		}
	}

	for (size_t i = 0; i < listFire.size(); i++) {
		listFire[i]->Update(dt, coObjects);
		/*if (!listFire[i]->CheckObjectInCamera() || listFire[i]->isFinish)
			listFire.erase(listFire.begin() + i);*/
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void FirePlant::ShootFire(int marioRange)
{
	switch (marioRange)
	{
	case LEFT_TOP_SIDE_NEAR:
		vyFire = -FIRE_ENEMY_SPEED_Y_NEAR;
		vxFire = FIRE_ENEMY_SPEED_X_NEAR;
		this->nx = -1;
		break;
	case LEFT_TOP_SIDE_FAR:
		vyFire = -FIRE_ENEMY_SPEED_Y_FAR;
		vxFire = FIRE_ENEMY_SPEED_X_FAR;
		this->nx = -1;
		break;
	case LEFT_BOTTOM_SIDE_NEAR:
		vyFire = FIRE_ENEMY_SPEED_Y_NEAR;
		vxFire = FIRE_ENEMY_SPEED_X_NEAR;
		this->nx = -1;
		break;
	case LEFT_BOTTOM_SIDE_FAR:
		vyFire = FIRE_ENEMY_SPEED_Y_FAR;
		vxFire = FIRE_ENEMY_SPEED_X_FAR;
		this->nx = -1;
		break;
	case RIGHT_TOP_SIDE_NEAR:
		vyFire = -FIRE_ENEMY_SPEED_Y_NEAR;
		vxFire = FIRE_ENEMY_SPEED_X_NEAR;
		this->nx = 1;
		break;
	case RIGHT_TOP_SIDE_FAR:
		vyFire = -FIRE_ENEMY_SPEED_Y_FAR;
		vxFire = FIRE_ENEMY_SPEED_X_FAR;
		this->nx = 1;
		break;
	case RIGHT_BOTTOM_SIDE_NEAR:
		vyFire = FIRE_ENEMY_SPEED_Y_NEAR;
		vxFire = FIRE_ENEMY_SPEED_X_NEAR;
		this->nx = 1;
		break;
	case RIGHT_BOTTOM_SIDE_FAR:
		vyFire = FIRE_ENEMY_SPEED_Y_FAR;
		vxFire = FIRE_ENEMY_SPEED_X_FAR;
		this->nx = 1;
		break;
	}
}


void FirePlant::Render()
{
	int ani = -1;

	marioRange = GetCurrentMarioRange();

	if (model == FIRE_PLANT_RED){
		if (state == PIRANHAPLANT_STATE_SHOOTING)
		{
			if (marioRange == LEFT_TOP_SIDE_NEAR || marioRange == LEFT_TOP_SIDE_FAR)
				ani = FIRE_PLANT_RED_ANI_ATTACK_LEFT_TOP;
			if (marioRange == LEFT_BOTTOM_SIDE_NEAR || marioRange == LEFT_BOTTOM_SIDE_FAR)
				ani = FIRE_PLANT_RED_ANI_ATTACK_LEFT_BOTTOM;
			if (marioRange == RIGHT_TOP_SIDE_NEAR || marioRange == RIGHT_TOP_SIDE_FAR)
				ani = FIRE_PLANT_RED_ANI_ATTACK_RIGHT_TOP;
			if (marioRange == RIGHT_BOTTOM_SIDE_NEAR || marioRange == RIGHT_BOTTOM_SIDE_FAR)
				ani = FIRE_PLANT_RED_ANI_ATTACK_RIGHT_BOTTOM;
		}
		else if (state == PIRANHAPLANT_STATE_DARTING)
		{
			if (marioRange == LEFT_TOP_SIDE_NEAR || marioRange == LEFT_TOP_SIDE_FAR)
				ani = FIRE_PLANT_RED_ANI_LEFT_TOP;
			if (marioRange == LEFT_BOTTOM_SIDE_NEAR || marioRange == LEFT_BOTTOM_SIDE_FAR)
				ani = FIRE_PLANT_RED_ANI_LEFT_BOTTOM;
			if (marioRange == RIGHT_TOP_SIDE_NEAR || marioRange == RIGHT_TOP_SIDE_FAR)
				ani = FIRE_PLANT_RED_ANI_RIGHT_TOP;
			if (marioRange == RIGHT_BOTTOM_SIDE_NEAR || marioRange == RIGHT_BOTTOM_SIDE_FAR)
				ani = FIRE_PLANT_RED_ANI_RIGHT_BOTTOM;
		}
		else return;
	}
	else if (model == FIRE_PLANT_GREEN)
	{
		if (marioRange == LEFT_TOP_SIDE_NEAR || marioRange == LEFT_TOP_SIDE_FAR)
				ani = FIRE_PLANT_GREEN_ANI_LEFT_TOP;
		else if (marioRange == LEFT_BOTTOM_SIDE_NEAR || marioRange == LEFT_BOTTOM_SIDE_FAR)
				ani = FIRE_PLANT_GREEN_ANI_LEFT_BOTTOM;
		else if (marioRange == RIGHT_TOP_SIDE_NEAR || marioRange == RIGHT_TOP_SIDE_FAR)
				ani = FIRE_PLANT_GREEN_ANI_RIGHT_TOP;
		else if (marioRange == RIGHT_BOTTOM_SIDE_NEAR || marioRange == RIGHT_BOTTOM_SIDE_FAR)
				ani = FIRE_PLANT_GREEN_ANI_RIGHT_BOTTOM;
		else return;
	}
	else if (model == BITE_PLANT)
			ani = PIRANHAPLANT_ANI_DARTING;
	else return;

	animation_set->at(ani)->Render(x, y);
	for (size_t i = 0; i < listFire.size(); i++)
	{
		//if (!listFire[i]->CheckObjectInCamera() || !listFire.at(i)->isFinish)
			listFire[i]->Render();
	}

}

void FirePlant::SetState(int state)
{
	CGameObject::SetState(state);

		switch (state)
		{
		case PIRANHAPLANT_STATE_DARTING:
			vy = -PIRANHAPLANT_DARTING_SPEED;
			break;
		case PIRANHAPLANT_STATE_BITING:
			vy = 0;
			StartBitting();
			break;
		case PIRANHAPLANT_STATE_DEATH:
			vy = 0;
			StartDying();
			break;
		case PIRANHAPLANT_STATE_INACTIVE:
			vy = 0;
			StartBitting();
			break;
		case PIRANHAPLANT_STATE_SHOOTING:
			vy = 0;
			ShootFire(marioRange);
			FireBall *fire = new FireBall(this->x,
											this->y,
											this->vxFire,
											this->vyFire);
			fire->nx = this->nx;
			listFire.push_back(fire);
			break;
		}
}
void FirePlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (model != BITE_PLANT) {
		left = x;
		top = y;
		right = x + PIRANHAPLANT_BBOX_WIDTH;
		bottom = y + plantHeight;
	}
	else {
		right = left + FIRE_PLANT_BBOX_WIDTH;
		bottom = y + PIRANHAPLANT_BBOX_HEIGHT;
	} 
}


bool FirePlant::GetSafeZone()
{
	LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (abs(this->x - mario->x) <= DISTANCE_SAFE_ZONE)   
	{
			return true;
	}
	return false;
}

int FirePlant::GetCurrentMarioRange() {
	LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	if ( this->x > mario->x) {	//Plant ben phai Mario
		if (mario->y < this->y) {
			if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
			{
				return LEFT_TOP_SIDE_NEAR;
			}
			else
			{
				return LEFT_TOP_SIDE_FAR;
			}
		}
		else
		{
			if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
			{
				return LEFT_BOTTOM_SIDE_NEAR;
			}
			else
			{
				return LEFT_BOTTOM_SIDE_FAR;
			}
		}
	}
	
	else	//Plant ben trai Mario
	{
		if (mario->y < this->y) {
			if (mario->x - this->x  < DISTANCE_FROM_MARIO_TO_PLANT)
			{
				return RIGHT_TOP_SIDE_NEAR;
			}
			else
			{
				return RIGHT_TOP_SIDE_FAR;
			}
		}
		else 
		{
			if (mario->x - this->x  < DISTANCE_FROM_MARIO_TO_PLANT)
			{
				return RIGHT_BOTTOM_SIDE_NEAR;
			}
			else 
			{
				return RIGHT_BOTTOM_SIDE_FAR;
			}
		}
	}

}


FirePlant::~FirePlant()
{
}
