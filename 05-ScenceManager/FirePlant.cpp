#include "FirePlant.h"



FirePlant::FirePlant()
{
	//this->x = X;
	//this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_FIRE_PLANT_FROM_FILE));
	isFinish = false;
	timeHidding = 0;
	timeAttack = 0;
	timeAttackDelay = 0;
	this->marioRange = marioRange;
	maxDistanceY = this->y;
	SetState(FIRE_PLANT_STATE_GROW_UP);
}
void FirePlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (isFinish)
		return;
	x += dx;
	y += dy;

	if (y <= minDistanceY) {
		y = minDistanceY;
		vy = 0;
		SetState(FIRE_PLANT_STATE_ATTACK);
		timeAttack += dt;
		timeAttackDelay += dt;
	}
	else if (y >= maxDistanceY) {
		y = maxDistanceY;
		vy = 0;
		timeHidding += dt;
	}

	if (!GetSafeZone() && timeHidding > TIME_DELAY_GROW_UP){ //neu time tron trong ong > time delay & ko phai trong vung an toan thi grow up	{
		SetState(FIRE_PLANT_STATE_GROW_UP);
		timeHidding = 0;
	}

	//if (timeAttackDelay > TIME_ATTACK_DELAY) {
	//	if (listFirePlant.size() < 1)
	//		CreateFirePlant(marioRange);
	//	timeAttackDelay = 0;
	//}

	if (timeAttack > TIME_ATTACK) {
		SetState(FIRE_PLANT_STATE_HIDING);
		timeAttack = 0;
	}

	if (!(CheckObjectInCamera(this))) //ra khoi cam thi xoa plant
	{
		isFinish = false;
	}
	SetFireFly(marioRange); 


}
void FirePlant::SetFireFly(int marioRange)
{
	switch (marioRange)
	{
	case LEFT_TOP_SIDE_NEAR:
		vy = -FIRE_ENEMY_SPEED_Y_NEAR * dt;
		vx = -FIRE_ENEMY_SPEED_X_NEAR * dt;
		this->nx = -1;
		break;
	case LEFT_TOP_SIDE_FAR:
		vy = -FIRE_ENEMY_SPEED_Y_FAR * dt;
		vx = -FIRE_ENEMY_SPEED_X_FAR * dt;
		this->nx = -1;
		break;
	case LEFT_BOTTOM_SIDE_NEAR:
		vy = FIRE_ENEMY_SPEED_Y_NEAR * dt;
		vx = -FIRE_ENEMY_SPEED_X_NEAR * dt;
		this->nx = -1;
		break;
	case LEFT_BOTTOM_SIDE_FAR:
		vy = FIRE_ENEMY_SPEED_Y_FAR * dt;
		vx = -FIRE_ENEMY_SPEED_X_FAR * dt;
		this->nx = -1;
		break;
	case RIGHT_TOP_SIDE_NEAR:
		vy = -FIRE_ENEMY_SPEED_Y_NEAR * dt;
		vx = FIRE_ENEMY_SPEED_X_NEAR * dt;
		this->nx = 1;
		break;
	case RIGHT_TOP_SIDE_FAR:
		vy = -FIRE_ENEMY_SPEED_Y_FAR * dt;
		vx = FIRE_ENEMY_SPEED_X_FAR * dt;
		this->nx = 1;
		break;
	case RIGHT_BOTTOM_SIDE_NEAR:
		vy = FIRE_ENEMY_SPEED_Y_NEAR * dt;
		vx = FIRE_ENEMY_SPEED_X_NEAR * dt;
		this->nx = 1;
		break;
	case RIGHT_BOTTOM_SIDE_FAR:
		vy = FIRE_ENEMY_SPEED_Y_FAR * dt;
		vx = FIRE_ENEMY_SPEED_X_FAR * dt;
		this->nx = 1;
		break;
	}
}


void FirePlant::Render()
{
	int ani = -1;
	if (isFinish)
		return;
	marioRange = GetCurrentMarioRange();
	if (state == FIRE_PLANT_STATE_ATTACK)
	{
		if (marioRange == LEFT_TOP_SIDE_NEAR || marioRange == LEFT_TOP_SIDE_FAR)
		{
			ani = FIRE_PLANT_ANI_ATTACK_LEFT_TOP;
		}
		if (marioRange == LEFT_BOTTOM_SIDE_NEAR || marioRange == LEFT_BOTTOM_SIDE_FAR)
		{
			ani = FIRE_PLANT_ANI_ATTACK_LEFT_BOTTOM;
		}
		if (marioRange == RIGHT_TOP_SIDE_NEAR || marioRange == RIGHT_TOP_SIDE_FAR)
		{
			ani = FIRE_PLANT_ANI_ATTACK_RIGHT_TOP;
		}
		if (marioRange == RIGHT_BOTTOM_SIDE_NEAR || marioRange == RIGHT_BOTTOM_SIDE_FAR)
		{
			ani = FIRE_PLANT_ANI_ATTACK_RIGHT_BOTTOM;
		}
	}
	else
	{
		if (marioRange == LEFT_TOP_SIDE_NEAR || marioRange == LEFT_TOP_SIDE_FAR)
		{
			ani = FIRE_PLANT_ANI_LEFT_TOP;
		}
		if (marioRange == LEFT_BOTTOM_SIDE_NEAR || marioRange == LEFT_BOTTOM_SIDE_FAR)
		{
			ani = FIRE_PLANT_ANI_LEFT_BOTTOM;
		}
		if (marioRange == RIGHT_TOP_SIDE_NEAR || marioRange == RIGHT_TOP_SIDE_FAR)
		{
			ani = FIRE_PLANT_ANI_RIGHT_TOP;
		}
		if (marioRange == RIGHT_BOTTOM_SIDE_NEAR || marioRange == RIGHT_BOTTOM_SIDE_FAR)
		{
			ani = FIRE_PLANT_ANI_RIGHT_BOTTOM;
		}
	}


	animation_set->at(ani)->Render(x, y);
}

void FirePlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_PLANT_STATE_GROW_UP:
	{
		vy = -PLANT_SPEED_GROW_UP * dt;
		break;
	}
	case FIRE_PLANT_STATE_HIDING:
	{
		vy = PLANT_SPEED_HIDDING * dt;
		break;
	}
	case FIRE_PLANT_STATE_ATTACK:
	{
		isFinish = true;
		break;
	}
	}
}
void FirePlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + FIRE_BBOX_WIDTH;
	bottom = top + FIRE_BBOX_HEIGHT;
	if (isFinish)
		left = top = right = bottom = 0;
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

	if (mario->x < this->x && mario->y < this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_TOP_SIDE_NEAR;
		}
		if (this->x - mario->x >= DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_TOP_SIDE_FAR;
		}
	}
	if (mario->x < this->x && mario->y > this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_BOTTOM_SIDE_NEAR;
		}
		if (this->x - mario->x > DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_BOTTOM_SIDE_FAR;
		}

	}
	if (mario->x > this->x && mario->y < this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_TOP_SIDE_NEAR;
		}
		if (this->x - mario->x >= DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_TOP_SIDE_FAR;
		}
	}
	if (mario->x > this->x && mario->y > this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_BOTTOM_SIDE_NEAR;
		}
		if (this->x - mario->x >= DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_BOTTOM_SIDE_FAR;
		}
	}
}




void FirePlant::CreateFirePlant(int marioRange)
{
	marioRange = GetCurrentMarioRange();
	FirePlant* fireplant = new FirePlant();
	listFirePlant.push_back(fireplant);
}

FirePlant::~FirePlant()
{
}
