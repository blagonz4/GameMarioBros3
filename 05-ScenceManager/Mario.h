#pragma once
#include "Utils.h"
#include "GameObject.h"
#include "define.h"

//----------------------Mario------------------------
#define MARIO_WALKING_SPEED					0.00012f 
#define MARIO_WALKING_MAXSPEED				0.1f
#define MARIO_RUNNING_SPEED					0.00015f
#define MARIO_RUNNING_MAXSPEED				0.25f
#define MARIO_JUMP_SPEED					0.01f
#define MARIO_FLY_SPEED						0.02f
#define MARIO_JUMP_SPEED_PEEK				0.005f
#define MARIO_JUMP_DEFLECT_SPEED			0.09f
#define MARIO_GRAVITY						0.002f
#define MARIO_DIE_DEFLECT_SPEED				0.03f
#define MARIO_SLIDING_SPEED					1.5f

//----------------------Mario------------------------
#define MARIO_STATE_IDLE					0
#define MARIO_STATE_WALK_RIGHT				1000
#define MARIO_STATE_WALK_LEFT				2000
#define MARIO_STATE_RUN_RIGHT				3000
#define MARIO_STATE_RUN_LEFT				4000
#define MARIO_STATE_JUMP					5000
#define MARIO_STATE_FLY_RIGHT				6000
#define MARIO_STATE_FLY_LEFT				7000
#define MARIO_STATE_DIE						8000
#define MARIO_STATE_SIT						9000
#define MARIO_STATE_TURN					10000
#define MARIO_STATE_KICK					11000
#define MARIO_STATE_SHOOT_FIRE				12000
#define MARIO_STATE_HOLD					13000
#define MARIO_STATE_FLOAT					14000	//racoon
#define MARIO_STATE_SPIN					15000	//racoon
#define MARIO_STATE_RUN_MAXSPEED			16000
#define MARIO_STATE_WORLD_MAP				17000
//----------------------Mario SMALL------------------------
#define MARIO_ANI_SMALL_IDLE_RIGHT			0
#define MARIO_ANI_SMALL_IDLE_LEFT			1
#define MARIO_ANI_SMALL_WALK_RIGHT			2
#define MARIO_ANI_SMALL_WALK_LEFT			3
#define MARIO_ANI_SMALL_RUN_RIGHT			4
#define MARIO_ANI_SMALL_RUN_LEFT			5
#define MARIO_ANI_SMALL_TURN_RIGHT			6
#define MARIO_ANI_SMALL_TURN_LEFT			7
#define MARIO_ANI_SMALL_JUMP_RIGHT			8
#define MARIO_ANI_SMALL_JUMP_LEFT			9
#define MARIO_ANI_SMALL_FLY_RIGHT			10
#define MARIO_ANI_SMALL_FLY_LEFT			11
#define MARIO_ANI_SMALL_KICK_RIGHT			12
#define MARIO_ANI_SMALL_KICK_LEFT			13
#define MARIO_ANI_SMALL_HOLD_IDLE_RIGHT		14
#define MARIO_ANI_SMALL_HOLD_IDLE_LEFT		15
#define MARIO_ANI_SMALL_HOLD_WALK_RIGHT		16
#define MARIO_ANI_SMALL_HOLD_WALK_LEFT		17
#define MARIO_ANI_SMALL_HOLD_JUMP_RIGHT		18
#define MARIO_ANI_SMALL_HOLD_JUMP_LEFT		19
#define MARIO_ANI_SMALL_HOLD_RUN_RIGHT		20
#define MARIO_ANI_SMALL_HOLD_RUN_LEFT		21
//----------------------Mario BIG------------------------


#define MARIO_ANI_BIG_IDLE_RIGHT			22
#define MARIO_ANI_BIG_IDLE_LEFT				23
#define MARIO_ANI_BIG_WALK_RIGHT			24
#define MARIO_ANI_BIG_WALK_LEFT				25
#define MARIO_ANI_BIG_RUN_RIGHT				26
#define MARIO_ANI_BIG_RUN_LEFT				27
#define MARIO_ANI_BIG_JUMP_RIGHT			28
#define MARIO_ANI_BIG_JUMP_LEFT				29
#define MARIO_ANI_BIG_SIT_RIGHT				30
#define MARIO_ANI_BIG_SIT_LEFT				31
#define MARIO_ANI_BIG_TURN_RIGHT			32
#define MARIO_ANI_BIG_TURN_LEFT				33
#define MARIO_ANI_BIG_RUN_RIGHT_MAXSPEED	34
#define MARIO_ANI_BIG_RUN_LEFT_MAXSPEED		35
#define MARIO_ANI_BIG_FLY_RIGHT				36
#define MARIO_ANI_BIG_FLY_LEFT				37
#define MARIO_ANI_BIG_KICK_RIGHT			38
#define MARIO_ANI_BIG_KICK_LEFT				39
#define MARIO_ANI_BIG_HOLD_IDLE_RIGHT		40
#define MARIO_ANI_BIG_HOLD_IDLE_LEFT		41
#define MARIO_ANI_BIG_HOLD_WALK_RIGHT		42
#define MARIO_ANI_BIG_HOLD_WALK_LEFT		43
#define MARIO_ANI_BIG_HOLD_JUMP_RIGHT		44
#define MARIO_ANI_BIG_HOLD_JUMP_LEFT		45
#define MARIO_ANI_BIG_HOLD_RUN_RIGHT		46
#define MARIO_ANI_BIG_HOLD_RUN_LEFT			47
//----------------------Mario FIRE------------------------

#define MARIO_ANI_FIRE_IDLE_RIGHT			48
#define MARIO_ANI_FIRE_IDLE_LEFT			49
#define MARIO_ANI_FIRE_WALK_RIGHT			50
#define MARIO_ANI_FIRE_WALK_LEFT			51
#define MARIO_ANI_FIRE_RUN_RIGHT			52
#define MARIO_ANI_FIRE_RUN_LEFT				53
#define MARIO_ANI_FIRE_JUMP_RIGHT			54
#define MARIO_ANI_FIRE_JUMP_LEFT			55
#define MARIO_ANI_FIRE_SIT_RIGHT			56
#define MARIO_ANI_FIRE_SIT_LEFT				57
#define MARIO_ANI_FIRE_TURN_RIGHT			58
#define MARIO_ANI_FIRE_TURN_LEFT			59
#define MARIO_ANI_FIRE_RUN_RIGHT_MAXSPEED	60
#define MARIO_ANI_FIRE_RUN_LEFT_MAXSPEED	61
#define MARIO_ANI_FIRE_FLY_RIGHT			62
#define MARIO_ANI_FIRE_FLY_LEFT				63
#define MARIO_ANI_FIRE_KICK_RIGHT			64
#define MARIO_ANI_FIRE_KICK_LEFT			65
#define MARIO_ANI_FIRE_SHOOT_FIRE_RIGHT		66
#define MARIO_ANI_FIRE_SHOOT_FIRE_LEFT		67
#define MARIO_ANI_FIRE_HOLD_IDLE_RIGHT		68
#define MARIO_ANI_FIRE_HOLD_IDLE_LEFT		69
#define MARIO_ANI_FIRE_HOLD_WALK_RIGHT		70
#define MARIO_ANI_FIRE_HOLD_WALK_LEFT		71
#define MARIO_ANI_FIRE_HOLD_JUMP_RIGHT		72
#define MARIO_ANI_FIRE_HOLD_JUMP_LEFT		73
#define MARIO_ANI_FIRE_HOLD_RUN_RIGHT		74
#define MARIO_ANI_FIRE_HOLD_RUN_LEFT		75
//----------------------Mario RACOON------------------------

#define MARIO_ANI_RACOON_IDLE_RIGHT			76
#define MARIO_ANI_RACOON_IDLE_LEFT			77
#define MARIO_ANI_RACOON_WALK_RIGHT			78
#define MARIO_ANI_RACOON_WALK_LEFT			79
#define MARIO_ANI_RACOON_RUN_RIGHT			80
#define MARIO_ANI_RACOON_RUN_LEFT			81
#define MARIO_ANI_RACOON_JUMP_RIGHT			82
#define MARIO_ANI_RACOON_JUMP_LEFT			83
#define MARIO_ANI_RACOON_SIT_RIGHT			84
#define MARIO_ANI_RACOON_SIT_LEFT			85
#define MARIO_ANI_RACOON_TURN_RIGHT			86
#define MARIO_ANI_RACOON_TURN_LEFT			87
#define MARIO_ANI_RACOON_RUN_RIGHT_MAXSPEED	88
#define MARIO_ANI_RACOON_RUN_LEFT_MAXSPEED	89
#define MARIO_ANI_RACOON_FLY_RIGHT			90
#define MARIO_ANI_RACOON_FLY_LEFT			91
#define MARIO_ANI_RACOON_KICK_RIGHT			92
#define MARIO_ANI_RACOON_KICK_LEFT			93
#define MARIO_ANI_RACOON_SPIN_RIGHT			94
#define MARIO_ANI_RACOON_SPIN_LEFT			95
#define MARIO_ANI_RACOON_HOLD_IDLE_RIGHT	96
#define MARIO_ANI_RACOON_HOLD_IDLE_LEFT		97
#define MARIO_ANI_RACOON_HOLD_WALK_RIGHT	98
#define MARIO_ANI_RACOON_HOLD_WALK_LEFT		99
#define MARIO_ANI_RACOON_HOLD_JUMP_RIGHT	100
#define MARIO_ANI_RACOON_HOLD_JUMP_LEFT		101
#define MARIO_ANI_RACOON_HOLD_RUN_RIGHT		102
#define MARIO_ANI_RACOON_HOLD_RUN_LEFT		103

#define MARIO_ANI_DIE						104
#define MARIO_ANI_SMALL_RUN_RIGHT_MAXSPEED	105
#define MARIO_ANI_SMALL_RUN_LEFT_MAXSPEED	106
#define MARIO_ANI_RACOON_FLOAT_RIGHT		107
#define MARIO_ANI_RACOON_FLOAT_LEFT			108

#define MARIO_ANI_SMALL_WORLD_MAP			109
#define MARIO_ANI_BIG_WORLD_MAP				110
#define MARIO_ANI_RACOON_WORLD_MAP			111
#define MARIO_ANI_FIRE_WORLD_MAP			112

//--------------------------HITBOX---------------------
#define	MARIO_LEVEL_SMALL					51
#define	MARIO_LEVEL_BIG						52
#define MARIO_LEVEL_FIRE					53
#define MARIO_LEVEL_RACOON					54
#define MARIO_BIG_BBOX_WIDTH				15
#define MARIO_BIG_BBOX_HEIGHT				27
#define MARIO_SMALL_BBOX_WIDTH				13
#define MARIO_SMALL_BBOX_HEIGHT				15
#define MARIO_BIG_BBOX_SIT_WIDTH			15
#define MARIO_BIG_BBOX_SIT_HEIGHT			18
#define MARIO_RACOON_BBOX_SPIN_WIDTH		23
#define TIME_UNTOUCHABLE_SHORT				500
#define	TIME_UNTOUCHABLE_LONG				3000
#define MARIO_LIMIT_JUMP_TIME				450
#define MARIO_LIMIT_FLY_TIME				400


class CMario : public CGameObject
{

	DWORD limitjump_start;
	DWORD limitfly_start;
	DWORD timeTransform; 
	DWORD turning_start;
	DWORD falling_start;
	float start_x;			// initial position of Mario at scene
	float start_y; 
	int score;
	int coinCollect;
public: 
	bool isOnGround = true;
	bool isSitting;
	bool isHolding;
	bool isTransformToBig = false;
	bool isTransformToRacoon = false;
	bool isJumping = false;
	bool isFlying = false;
	bool isSpinning = false;
	bool isFalling = false;
	int gotCard;
	int marioLimitJumpTime = MARIO_LIMIT_JUMP_TIME;
	CMario(float x = 0.0f, float y = 0.0f);
	vector<LPGAMEOBJECT> listFire;
	vector<LPGAMEOBJECT> listEffect;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	int GetLevel(){return level;}
	void SetState(int state);
	void PlusScore(int score) { this->score += score; }
	void PlusCoinCollect(int coin) { this->coinCollect += coin; }
	int GetScore() { return score; }
	int GetCoinCollect() { return coinCollect; }
	void SetLevel(int l) { level = l; }
	void StartLimitJump() { isJumping = true; limitjump_start = GetTickCount(); }
	void StartLimitFly() { isFlying = true; limitfly_start = GetTickCount(); }
	void StartFalling() { isFalling = true; falling_start = GetTickCount(); }
	void StartSpinning() { turning_start = GetTickCount(); isSpinning = true; }
	void Reset();
	void ShowEffectPoint(CGameObject* obj, float model);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

