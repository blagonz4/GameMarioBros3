#include "FirePlant.h"
#include "Pipe.h"
#include "Mario.h"
#include "FireBall.h"

FirePlant::FirePlant(CMario* mario,float model)
{
	this->mario = mario;
	timeHidding = 0;
	timeAttack = 0;
	timeDelayAttack = 0;
	this->marioRange = marioRange;
	SetState(FIRE_PLANT_STATE_HIDING);
	eType = Type::FIREPLANT;
	this->model = model;
}
void FirePlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	//if (CheckObjectInCamera(this))//TRONG CAMERA THI CHUI LEN
	//	if (!isGrowUp && isHiding) {
	//		SetState(FIRE_PLANT_STATE_GROW_UP);
	//	}
	//
	//if (!CheckObjectInCamera(this)) {
	//	SetState(FIRE_PLANT_STATE_HIDING);
	//	timeAttack = 0;
	//	timeDelayAttack = 0;
	//	timeToHide = 0;
	//	vy = 0;
	//}
	//	
	//
	//if (isGrowUp) {//DANG CHUI LEN SE SET TIMER
	//	vy = -MARIO_GRAVITY * dt;
	//	if (y < 338 && model == FIRE_PLANT_RED) {
	//		vy = 0;
	//		SetState(FIRE_PLANT_STATE_ATTACK);
	//	}
	//	if (y < 344 && model == FIRE_PLANT_GREEN) {
	//		vy = 0;
	//		SetState(FIRE_PLANT_STATE_ATTACK);
	//	}
	//	if (y < 361 && model == BITE_PLANT) {
	//		vy = 0;
	//		SetState(FIRE_PLANT_STATE_ATTACK);
	//	}
	//}
	//
	//if (isAttacking) {
	//	if (listFire.size() < 1 && model != BITE_PLANT) {
	//		FireBall *fire = new FireBall(this->x,
	//										this->y,
	//										this->vxFire,
	//										this->vyFire);
	//		fire->nx = this->nx;
	//		listFire.push_back(fire);
	//	}
	//	timeAttack += dt;
	//}
	//
	//
	//if (timeAttack >= 1000) {//KHAC LUA XONG HOLD 1 XIU		
	//	SetState(FIRE_PLANT_STATE_HIDING);	
	//	timeAttack = 0;
	//}
	//
	//if (listFire.size() == 1 )
	//	if (CheckObjectInCamera(this)) {
	//		timeToHide += dt;
	//		timeDelayAttack += dt;
	//	}
	//
	//
	//if (timeToHide > 1000) {//HOLD XONG CHUI XUONG LAI
	//	vy = MARIO_GRAVITY * dt;
	//	if (y > 380 && model == FIRE_PLANT_RED) {
	//		vy = 0;			
	//	}
	//	if (y >368 && model == FIRE_PLANT_GREEN) {
	//		vy = 0;
	//	}
	//	if (y < 384 && model == BITE_PLANT) {
	//		vy = 0;
	//	}
	//	if (timeToHide > 3000) {
	//		SetState(FIRE_PLANT_STATE_HIDING);
	//		timeToHide = 0;
	//	}
	//}

	if (isHiding) {
		SetState(FIRE_PLANT_STATE_GROW_UP);
	}

	if (isGrowUp) {//DANG CHUI LEN SE SET TIMER
		vy = -MARIO_GRAVITY * dt;
		if (y < 338 && model == FIRE_PLANT_RED) {
			vy = 0;
			SetState(FIRE_PLANT_STATE_ATTACK);
		}
		if (y < 344 && model == FIRE_PLANT_GREEN) {
			vy = 0;
			SetState(FIRE_PLANT_STATE_ATTACK);
		}
		if (y < 361 && model == BITE_PLANT) {
			vy = 0;
			SetState(FIRE_PLANT_STATE_ATTACK);
		}
	}
	if (isAttacking) {
		if (listFire.size() < 1 && model != BITE_PLANT) {
		//if (model != BITE_PLANT){
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			FireBall *fire = new FireBall(this->x,
											this->y,
											this->vxFire,
											this->vyFire);
			fire->nx = this->nx;	
			scene->TurnIntoUnit(fire);
			listFire.push_back(fire);
		}
		timeAttack += dt;
	}
	
	
	if (timeAttack >= 1000) {//KHAC LUA XONG HOLD 1 XIU		
		SetState(FIRE_PLANT_STATE_HIDING);	
		timeAttack = 0;
	}
	
	if (listFire.size() == 1 || model == BITE_PLANT){
		timeToHide += dt;
		//timeDelayAttack += dt;
	}

	if (timeToHide > 5000) {//HOLD XONG CHUI XUONG LAI
		vy = MARIO_GRAVITY * dt;
		if (y > 380 && model == FIRE_PLANT_RED) {
			vy = 0;			
			SetState(FIRE_PLANT_STATE_HIDING); timeToHide = 0;

		}
		if (y >368 && model == FIRE_PLANT_GREEN) {
			vy = 0;
			SetState(FIRE_PLANT_STATE_HIDING); timeToHide = 0;
		}
		if (y > 384 && model == BITE_PLANT) {
			vy = 0;
			SetState(FIRE_PLANT_STATE_HIDING);
			timeToHide = 0;
		}
		//if (timeToHide > 3000) {
		//	SetState(FIRE_PLANT_STATE_HIDING);
		//	timeToHide = 0;
		//}
	}

	for (size_t i = 0; i < listFire.size(); i++) {
		if (!listFire[i]->CheckObjectInCamera(listFire[i]) || listFire[i]->isFinish)
			listFire.erase(listFire.begin() + i);
		////listFire[i]->Update(dt, coObjects);
		//	//delete listFire[i];
	}

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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
void FirePlant::ShootFire(int marioRange)
{
	switch (marioRange)
	{
	case LEFT_TOP_SIDE_NEAR:
		vyFire = -FIRE_ENEMY_SPEED_Y_NEAR * dt;
		vxFire = FIRE_ENEMY_SPEED_X_NEAR * dt;
		this->nx = -1;
		break;
	case LEFT_TOP_SIDE_FAR:
		vyFire = -FIRE_ENEMY_SPEED_Y_FAR * dt;
		vxFire = FIRE_ENEMY_SPEED_X_FAR * dt;
		this->nx = -1;
		break;
	case LEFT_BOTTOM_SIDE_NEAR:
		vyFire = FIRE_ENEMY_SPEED_Y_NEAR * dt;
		vxFire = FIRE_ENEMY_SPEED_X_NEAR * dt;
		this->nx = -1;
		break;
	case LEFT_BOTTOM_SIDE_FAR:
		vyFire = FIRE_ENEMY_SPEED_Y_FAR * dt;
		vxFire = FIRE_ENEMY_SPEED_X_FAR * dt;
		this->nx = -1;
		break;
	case RIGHT_TOP_SIDE_NEAR:
		vyFire = -FIRE_ENEMY_SPEED_Y_NEAR * dt;
		vxFire = FIRE_ENEMY_SPEED_X_NEAR * dt;
		this->nx = 1;
		break;
	case RIGHT_TOP_SIDE_FAR:
		vyFire = -FIRE_ENEMY_SPEED_Y_FAR * dt;
		vxFire = FIRE_ENEMY_SPEED_X_FAR  * dt;
		this->nx = 1;
		break;
	case RIGHT_BOTTOM_SIDE_NEAR:
		vyFire = FIRE_ENEMY_SPEED_Y_NEAR * dt;
		vxFire = FIRE_ENEMY_SPEED_X_NEAR * dt;
		this->nx = 1;
		break;
	case RIGHT_BOTTOM_SIDE_FAR:
		vyFire = FIRE_ENEMY_SPEED_Y_FAR * dt;
		vxFire = FIRE_ENEMY_SPEED_X_FAR * dt;
		this->nx = 1;
		break;
	}
}


void FirePlant::Render()
{
	int ani = -1;
	//if (isFinish)
	//	return;
	marioRange = GetCurrentMarioRange();

	if (model == FIRE_PLANT_RED){
		if (state == FIRE_PLANT_STATE_ATTACK)
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
		else if (state == FIRE_PLANT_STATE_GROW_UP)
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
			ani = BITE_PLANT_ANI_ATTACK;
	else return;

	//for (size_t i = 0; i < listFire.size(); i++)
	//{
	//	if (CheckObjectInCamera(listFire.at(i)) || !listFire.at(i)->isFinish)
	//		listFire[i]->Render();
	//}

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void FirePlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_PLANT_STATE_HIDING:
	{
		isGrowUp = false;
		isAttacking = false;
		isHiding = true;
		break;
	}
	case FIRE_PLANT_STATE_GROW_UP:
	{		
		isGrowUp = true;
		isHiding = false;
		isAttacking = false;
		break;
	}
	case FIRE_PLANT_STATE_ATTACK:
	{
		ShootFire(marioRange);
		isGrowUp = false;
		isAttacking = true;
		isHiding = false;
		break;
	}
	}
}
void FirePlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == FIRE_PLANT_STATE_GROW_UP || state == FIRE_PLANT_STATE_ATTACK) {
		right = left + FIRE_PLANT_BBOX_WIDTH;
		bottom = top + FIRE_PLANT_RED_BBOX_HEIGHT;
	}
	else {
		right = left + FIRE_PLANT_BBOX_WIDTH;
		bottom = y;
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

	if (mario->x < this->x && mario->y < this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_TOP_SIDE_NEAR;
		}
		else // (this->x - mario->x >= DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_TOP_SIDE_FAR;
		}
	}
	else if (mario->x < this->x && mario->y > this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_BOTTOM_SIDE_NEAR;
		}
		else// (this->x - mario->x > DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_BOTTOM_SIDE_FAR;
		}

	}
	else if (mario->x > this->x && mario->y < this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_TOP_SIDE_NEAR;
		}
		else// (this->x - mario->x >= DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_TOP_SIDE_FAR;
		}
	}
	else  //(mario->x > this->x && mario->y > this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_BOTTOM_SIDE_NEAR;
		}
		else  //(this->x - mario->x >= DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_BOTTOM_SIDE_FAR;
		}
	}
}


FirePlant::~FirePlant()
{
}
