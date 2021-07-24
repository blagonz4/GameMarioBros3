#pragma once
#include "Utils.h"
#include "GameObject.h"
#include "define.h"

//----------------------Mario------------------------
#define MARIO_WALKING_SPEED					0.08f 
#define MARIO_INTRO_WALKING_SPEED			0.005f 
#define MARIO_WALKING_MAXSPEED				0.10f
#define MARIO_RUNNING_SPEED					0.12f
#define MARIO_RUNNING_MAXSPEED				0.25f
#define MARIO_JUMP_SPEED_MIN				0.15f
#define MARIO_JUMP_SPEED_MAX				0.27f
#define MARIO_FLY_SPEED						0.13f
#define MARIO_JUMP_SPEED_PEEK				0.005f
#define MARIO_JUMP_DEFLECT_SPEED			0.5f
#define MARIO_GRAVITY						0.002f
#define MARIO_DIE_DEFLECT_SPEED				0.8f
#define MARIO_ACCELERATION_JUMP				0.0005f
#define MARIO_SLIDING_SPEED					1.5f
#define TIME_SPINNING						500
#define TIME_TRANSFORM						200
#define TAIL_SIZE							15
#define MARIO_RUNNING_STACK_TIME			250
#define MARIO_SLOW_TIME						800
#define MARIO_ACCELERATION					0.0003f
#define MARIO_SIT_PUSH_BACK					9
#define MARIO_SLOW_STACK_TIME				250
#define MARIO_KICKING_TIME					200
#define MARIO_JUMPING_TIME					200
#define MARIO_SHOOTING_TIME					200
#define MARIO_TURNING_STATE_TIME			70
#define MARIO_TURNING_TAIL_TIME				350
#define MARIO_UNTOUCHABLE_TIME				2500
#define MARIO_FLAPPING_TIME					200
#define MARIO_SLOW_FALLING_SPEED			0.03f
#define MARIO_FLYING_TIME					1500
#define MARIO_TAIL_FLYING_TIME				250
#define MARIO_DEFAULT_LIFE					3
#define PLAYER_SPEED						0.1f
#define MARIO_DEFLECT_MUSIC_BRICK			0.8f
#define MARIO_CHANGE_SCENE_SPEED			1.4f
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

#define MARIO_ANI_BIG_JUMP_RIGHT_DOWN		109
#define MARIO_ANI_BIG_JUMP_LEFT_DOWN		110
#define MARIO_ANI_FIRE_JUMP_RIGHT_DOWN		111
#define MARIO_ANI_FIRE_JUMP_LEFT_DOWN		112
#define MARIO_ANI_RACOON_JUMP_RIGHT_DOWN	113
#define MARIO_ANI_RACOON_JUMP_LEFT_DOWN		114

#define MARIO_ANI_SMALL_WORLD_MAP			115
#define MARIO_ANI_BIG_WORLD_MAP				116
#define MARIO_ANI_RACOON_WORLD_MAP			117
#define MARIO_ANI_FIRE_WORLD_MAP			118

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
#define ENEMY_PUSH_BACK						3.f

class CMario : public CGameObject {
	DWORD untouchable_start;
	DWORD kicking_start;
	DWORD shooting_start;
	DWORD delay_start;
	DWORD turning_start;
	DWORD turning_state_start;
	DWORD flapping_start;
	DWORD running_start;
	DWORD running_stop;
	DWORD slow_start = 0;
	DWORD tailflying_start;
	DWORD transforming_start;
	DWORD fly_start = 0;
	DWORD delay_jump = 0;
public:
	float start_x;			// initial position of Mario at scene
	float start_y;
	bool cgLeft, cgRight, cgUp, cgDown;
	vector <LPGAMEOBJECT> listFire;
	vector <LPGAMEOBJECT> listEffect;
	float lastStandingY = 0;
	float limitJumpVelocity = MARIO_JUMP_SPEED_MAX;
	bool isWannaDown = false;

	int score;
	int life = MARIO_DEFAULT_LIFE;
	int coinCollect;
	int gotCard = 0;
	int turning_state = 0;
	int level;
	vector <int> listCards;

	int RunningStacks = 0;
	int prelevel;
	int untouchable;
	//state
	bool isOnGround = false;
	bool isInPipe = false;

	//sit
	bool isReadyToSit = true;
	bool isSitting = false;

	//high-jump
	bool isReadyToJump = true;
	bool isJumping = false;
	bool isChangeDirection = false;
	bool isDeflect = false;

	//using tail
	bool isTurningTail = false;
	bool isFlapping = false;
	bool isTailFlying = false;

	//shoot
	bool isShooting = false;
	bool isReadyToShoot = true;
	int	ShootTimes = 0;

	//hold
	bool isHolding = false;
	bool isReadyToHold = false;

	//kick
	bool isKicking = false;
	bool isReadyToKick = true;

	//run
	bool isRunning = false;
	bool isReadyToRun = true;
	//fly
	bool isFlying = false;
	//trasnforming
	bool isTransforming = false;
	bool transformState = false;

	CMario(float x = 0.0f, float y = 0.0f);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void CMario::TimingFlag();
	void BasicRenderLogicsForAllLevel(int& ani,
		int ani_idle_right, int ani_idle_left, int ani_jump_down_right, int ani_jump_down_left,
		int ani_baking_right, int ani_baking_left, int ani_walking_right, int ani_walking_left, int ani_kicking_right, int ani_kicking_left);

	void RenderJumping(int& ani, int ani_jump_up_right, int ani_jump_up_left, int ani_jump_down_right, int ani_jump_down_left);
	void PlusScore(int score) { this->score += score; }
	void PlusCoinCollect(int coin) { this->coinCollect += coin; }
	int GetScore() { return score; }
	int GetCoinCollect() { return coinCollect; }
	void SetState(int State);
	void SetLevel(int level) { this->level = level; }
	int GetLevel() { return this->level; }
	//timer
	void DelayShooting() { delay_start = GetTickCount(); isKicking = true; }
	void StartKicking() { kicking_start = GetTickCount(); isKicking = true; }
	void StartRunning() { running_start = GetTickCount(); isRunning = true; }
	void StopRunning() { running_stop = GetTickCount(); isRunning = false; }
	void StartShooting() { shooting_start = GetTickCount(); isShooting = true; }
	void StartSlowDown() { slow_start = GetTickCount(); isReadyToRun = false; }
	void StartTurning() { 
		turning_start = GetTickCount(); 
		isTurningTail = true; 
		turning_state_start = GetTickCount(); 
		turning_state = 1; 
	}
	void StartFlapping()
	{
		flapping_start = GetTickCount();
		isFlapping = true;
	}
	void StartTailFlying()
	{
		tailflying_start = GetTickCount();
		isTailFlying = true;
	}
	void StartFlying()
	{
		fly_start = GetTickCount();
	}
	void StartTransforming() { transforming_start = GetTickCount(); isTransforming = true; }
	void StartDelayJump() { delay_jump = GetTickCount(); isJumping = false; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Reset();
	void ShowEffectPoint(CGameObject* obj, float model);

	void SetMove(bool cLeft, bool cUp, bool cRight, bool cDown)
	{
		cgLeft = cLeft;
		cgRight = cRight;
		cgUp = cUp;
		cgDown = cDown;
	};
	void GetMove(bool& cLeft, bool& cUp, bool& cRight, bool& cDown)
	{
		cLeft = cgLeft;
		cRight = cgRight;
		cUp = cgUp;
		cDown = cgDown;
	};
	void BeingAttacked();
};