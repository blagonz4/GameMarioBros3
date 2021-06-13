#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iomanip>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <algorithm>

using namespace std;


//-----------------------Key Control--------------------------





//-----------------------main
#define WINDOW_CLASS_NAME L"Super Mario Bros 3"
#define MAIN_WINDOW_TITLE L"Super Mario Bros 3"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 300  
#define SCREEN_HEIGHT 300

#define MAX_FRAME_RATE 120


//------------------------ENUM TYPE---------------------------------
enum Type {
	MARIO = 0,

	PLATFORM = 1,
	GOOMBA = 2,
	KOOPAS = 3,
	COLORBLOCK = 4,
	FIRE = 5,
	FIREPLANT = 6,
	PIPE = 7,
	QUESTIONBRICK = 8,
	GOLDBRICK = 9,
	COIN = 10,
	PSWITCH = 11,
	MUSHROOM = 12,
	LEAF = 13,

	EFFECT_DISAPPEAR = 14,
	EFFECT_TAIL_HIT	= 15,
	EFFECT_BROKEN_BRICK = 16,
	EFFECT_COIN	= 17,
	EFFECT_POINT = 18,
	EFFECT_1UP = 19,

	MUSHROOM_SYMBOL = 20,
	FLOWER = 21,
	STAR = 22,
	BOX = 23,
	
	MUSHROOM_POWER = 50,
	MUSHROOM_1_UP = 51,
	PORTAL = 52,
};

enum ObjectType {
	MISC = 1,
	ENEMY = 2,
	ITEM = 3,
};

//-------------------------------map----------------------------
#define TILE_WIDTH 16.0f
#define TILE_HEIGHT 16.0f
#define COLOR_BLOCK_GET_THROUGH 100
//----------------------------Game----------------------------
#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024
//----------------------------Camera----------------------------
#define DISTANCE_TO_MARIO_X					120
#define DISTANCE_TO_MARIO_Y					5
#define DISTANCE_MARIO_FLY_THROUGH_SKY_Y	250
#define CAMERA_COORDINATE_Y					220
#define CAMERA_ROOT_Y						0
//-------------------------------------------------------------
#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2
//GameObject
#define ID_TEX_BBOX -100	

//------------------------------PlayScene
#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_DRAWMAP			7

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_PLATFORM			1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_COLOR_BLOCK			4
#define OBJECT_TYPE_FIRE_BALL			5
#define OBJECT_TYPE_FIRE_PLANT			6
#define OBJECT_TYPE_PIPE				7
#define OBJECT_TYPE_QUESTION_BRICK		8
#define OBJECT_TYPE_GOLD_BRICK			9
#define OBJECT_TYPE_COIN				10
#define OBJECT_TYPE_MUSHROOM			11
#define OBJECT_TYPE_LEAF				12
#define OBJECT_TYPE_BOX					13
#define OBJECT_TYPE_PORTAL				52
#define OBJECT_TYPE_BUSH				53
#define OBJECT_TYPE_START				54
#define OBJECT_TYPE_SCENE				55
#define OBJECT_TYPE_CARD				56

#define MAX_SCENE_LINE					1024
#define WORLDMAP						0
#define SCENE_TEST						1
#define WORLD1_1_1						2
#define TEXID_FONT35					35
#define GAME_TIME_LIMIT					999999
//-------------------------------Board-------------------
#define TEXID_BOARD								45
#define DISTANCE_FROM_BOTTOM_CAM_TO_TOP_BOARD	90.f
//-----------------------------Font----------------------
#define TEXID_FONT						55
#define TILE_FONT_WIDTH					8
#define TILE_FONT_A_HEIGHT				8
#define TILE_FONT_NUMBER_HEIGHT			7
//-----------------------------SceneWorldMap-------------------
#define SCENE_STATE_1					0
#define SCENE_STATE_2					1
#define SCENE_STATE_3					2
#define SCENE_STATE_4					3
#define SCENE_STATE_5					4
#define SCENE_STATE_6					5

#define SCENE_ANI_1					0
#define SCENE_ANI_2					1
#define SCENE_ANI_3					2
#define SCENE_ANI_4					3
#define SCENE_ANI_5					4
#define SCENE_ANI_6					5
//-----------------------------Goomba---------------------------
#define GOOMBA_WALKING_SPEED					0.005f

#define GOOMBA_BBOX_WIDTH						16
#define GOOMBA_WING_BBOX_WIDTH					20
#define GOOMBA_BBOX_HEIGHT						15
#define GOOMBA_WING_BBOX_HEIGHT					24
#define GOOMBA_BBOX_HEIGHT_DIE					9

#define GOOMBA_MODEL_NORMAL						1
#define GOOMBA_MODEL_WING						2


#define GOOMBA_STATE_DIE						100

#define GOOMBA_ANI_DIE							0
#define GOOMBA_ANI_WING_DIE						1
#define GOOMBA_ANI_WALKING						2
#define GOOMBA_ANI_WING_WALKING_WITH_OUT_WING	3
#define GOOMBA_ANI_WING_WALKING					4
#define GOOMBA_ANI_WING_JUMPING					5
#define GOOMBA_ANI_WING_FALLING					6

#define GOOMBA_JUMPING_SPEED					0.25f
#define TIME_TO_DIE								500
//-----------------------------Koopas----------------------------
#define KOOPAS_DEFEND_TIME			100000
#define KOOPAS_DEFEND_HITBOX		16
#define KOOPAS_WALK_HITBOX_WIDTH	16
#define KOOPAS_WALK_HITBOX_HEIGHT	27

#define KOOPAS_MODEL_RED			1
#define KOOPAS_MODEL_GREEN			2
#define KOOPAS_MODEL_GREEN_WING		3

#define KOOPAS_WALKING_SPEED		0.003f
#define	KOOPAS_FLY_SPEED			0.04f
#define KOOPAS_BALL_SPEED			0.01f
#define KOOPAS_BBOX_WIDTH			16
#define KOOPAS_BBOX_HEIGHT			26
#define KOOPAS_BBOX_HEIGHT_DIE		16

#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_DEFEND			200
#define KOOPAS_STATE_BALL			300
#define KOOPAS_STATE_REVIVE			400	//come back up side down
#define KOOPAS_STATE_DIE			500
#define KOOPAS_STATE_FLY			600

#define KOOPAS_ANI_RED_WALKING_LEFT			0
#define KOOPAS_ANI_RED_WALKING_RIGHT		1
#define KOOPAS_ANI_RED_DEFEND				2
#define KOOPAS_ANI_RED_BALL					3		
#define KOOPAS_ANI_GREEN_WALKING_LEFT		4
#define KOOPAS_ANI_GREEN_WALKING_RIGHT		5
#define KOOPAS_ANI_GREEN_DEFEND				6
#define KOOPAS_ANI_GREEN_BALL				7		
#define KOOPAS_ANI_GREEN_FLY_RIGHT			8
#define KOOPAS_ANI_GREEN_FLY_LEFT			9
#define KOOPAS_ANI_RED_DIE					10
#define KOOPAS_ANI_GREEN_DIE				11
//-----------------------------FIRE PLANT----------------------------
#define DISTANCE_SAFE_ZONE				25

#define LOAD_FIRE_PLANT_FROM_FILE		 6

#define LEFT_TOP_SIDE_NEAR				 1
#define LEFT_TOP_SIDE_FAR				 2
#define LEFT_BOTTOM_SIDE_NEAR			 3
#define LEFT_BOTTOM_SIDE_FAR			 4
#define RIGHT_TOP_SIDE_NEAR				 5
#define RIGHT_TOP_SIDE_FAR				 6
#define RIGHT_BOTTOM_SIDE_NEAR			 7
#define RIGHT_BOTTOM_SIDE_FAR			 8
#define LEFT_SIDE						 9
#define RIGHT_SIDE						10

#define FIRE_ENEMY_SPEED_Y_FAR			0.003f
#define FIRE_ENEMY_SPEED_Y_NEAR			0.0025f
#define FIRE_ENEMY_SPEED_X_NEAR			0.003f
#define FIRE_ENEMY_SPEED_X_FAR			0.003f

#define FIRE_PLANT_BBOX_WIDTH			16
#define FIRE_PLANT_RED_BBOX_HEIGHT		32
#define FIRE_PLANT_GREEN_BBOX_HEIGHT	24
//----------------------Red Plant---------------------
#define	FIRE_PLANT_RED_ANI_LEFT_TOP					0
#define FIRE_PLANT_RED_ANI_LEFT_BOTTOM				1
#define FIRE_PLANT_RED_ANI_RIGHT_TOP				2
#define FIRE_PLANT_RED_ANI_RIGHT_BOTTOM				3
#define FIRE_PLANT_RED_ANI_ATTACK_LEFT_TOP			4
#define FIRE_PLANT_RED_ANI_ATTACK_LEFT_BOTTOM		5
#define FIRE_PLANT_RED_ANI_ATTACK_RIGHT_TOP			6
#define FIRE_PLANT_RED_ANI_ATTACK_RIGHT_BOTTOM		7
//----------------------Green Plant---------------------
#define	FIRE_PLANT_GREEN_ANI_LEFT_TOP				8
#define FIRE_PLANT_GREEN_ANI_LEFT_BOTTOM			9
#define FIRE_PLANT_GREEN_ANI_RIGHT_TOP				10
#define FIRE_PLANT_GREEN_ANI_RIGHT_BOTTOM			11

//----------------------Bite Plant---------------------
#define	BITE_PLANT_ANI_ATTACK						12
#define PLANT_SPEED_GROW_UP				0.02f
#define PLANT_SPEED_HIDDING				0.02f
#define TIME_DELAY_GROW_UP				1500
#define TIME_ATTACK_DELAY				1000
#define TIME_ATTACK						1500
#define DISTANCE_FROM_MARIO_TO_PLANT	75

#define FIRE_PLANT_STATE_GROW_UP		1
#define FIRE_PLANT_STATE_ATTACK			2
#define FIRE_PLANT_STATE_HIDING			3

#define	FIRE_PLANT_RED					1
#define FIRE_PLANT_GREEN				2
#define BITE_PLANT						3
//-------------------------Pipe-----------
#define SHORT_PIPE 1
#define MEDIUM_PIPE 2
#define ANI_SHORT_PIPE 0
#define ANI_MEDIUM_PIPE 1
#define PIPE_BBOX_WIDTH 32
#define PIPE_SHORT_BBOX_HEIGHT 32 
#define PIPE_MEDIUM_BBOX_HEIGHT 48

//--------------------Brick------------------------
#define BRICK_BBOX_WIDTH					16
#define BRICK_BBOX_HEIGHT					16

#define QUESTION_BRICK_BBOX_WIDTH			16
#define QUESTION_BRICK_BBOX_HEIGHT			16

#define QUESTION_BRICK_ANI_BOX				0
#define QUESTION_BRICK_ANI_EMPTY			1
#define	QUESTION_BRICK_ANI_POWER_UP			2

#define QUESTION_BRICK_MIN_Y				15
#define QUESTION_BRICK_SPEED_UP				0.01f

#define QUESTION_BRICK_MODEL_COIN			1
#define QUESTION_BRICK_MODEL_POWER_UP		2
//---
#define	TIME_FROM_COIN_TO_BRICK				5000

#define GOLD_BRICK_ANI_BOX					0
#define GOLD_BRICK_ANI_UNBOX				1
#define GOLD_BRICK_ANI_IDLE_COIN			2
#define GOLD_BRICK_STATE_BOX				100
#define GOLD_BRICK_STATE_UNBOX				200
#define GOLD_BRICK_STATE_IDLE_COIN			300

#define GOLD_BRICK_MODEL_COIN					1
#define GOLD_BRICK_MODEL_PSWITCH				2
#define GOLD_BRICK_MODEL_MUSHROOM_1_UP			3
#define GOLD_BRICK_MODEL_MODEL_MANY_COIN		4
#define GOLD_BRICK_MODEL_POWER_UP				5
//---------------------PSwitch
#define PSWITCH_BBOX_HEIGHT					16
#define PSWITCH_BBOX_USED_HEIGHT			7
#define PSWITCH_BBOX_WIDTH					16
#define PSWITCH_ANI							0
#define PSWITCH_ANI_USED					1
#define LOAD_PSWITCH_FROM_TXT				11
#define PSWITCH_STATE_NORMAL				100
#define PSWITCH_STATE_USED					200
#define PSWITCH_SMALLER						10
//----------------------Leaf
#define LOAD_LEAF_FROM_TXT					13
#define LEAF_BBOX_WIDTH						16
#define LEAF_BBOX_HEIGHT					16
#define LEAF_GRAVITY						0.003f
#define LEAF_ANI_LEFT						0
#define LEAF_ANI_RIGHT						1
#define LEAF_MIN_Y							32
#define LEAF_MAX_RIGHT_X					24
#define LEAF_STATE_APPEAR					100
#define LEAF_SPEED_X						0.005f
//-----------------Mushroom
#define MUSHROOM_MODEL_RED						1
#define MUSHROOM_MODEL_GREEN					2
#define MUSHROOM_BBOX_WIDTH						16
#define MUSHROOM_BBOX_HEIGHT					16
#define MUSHROOM_RED_ANI						0
#define MUSHROOM_GREEN_ANI						1
#define MUSHROOM_GRAVITY						0.02f
#define MUSHROOM_SPEED							0.006f
#define MUSHROOM_SPEED_APPEAR_Y					0.01f
#define MUSHROOM_STATE_APPEAR					100
#define MUSHROOM_STATE_MOVING					200
#define LOAD_MUSHROOM_FROM_TXT					12

//----------------------Mario------------------------
#define MARIO_WALKING_SPEED					0.00012f 
#define MARIO_WALKING_MAXSPEED				0.1f
#define MARIO_RUNNING_SPEED					0.00015f
#define MARIO_RUNNING_MAXSPEED				0.25f
#define MARIO_JUMP_SPEED					0.01f
#define MARIO_FLY_SPEED						0.02f
#define MARIO_JUMP_SPEED_PEEK				0.005f
#define MARIO_JUMP_DEFLECT_SPEED			0.40f
#define MARIO_GRAVITY						0.002f
#define MARIO_DIE_DEFLECT_SPEED				0.3f
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
#define	TIME_UNTOUCHABLE_LONG				5000
#define MARIO_LIMIT_JUMP_TIME				450
#define MARIO_LIMIT_FLY_TIME				400


//----------------------FIRE BALL-------------------
#define FIRE_SPEED 0.008f
#define FIRE_GRAVITY 0.0006f
#define FIRE_BOUNCE_SPEED_Y  0.15f
#define FIRE_BBOX_WIDTH 8
#define FIRE_BBOX_HEIGHT 8
#define SHOOT_FIRE_RIGHT 0
#define SHOOT_FIRE_LEFT 1
#define LOAD_FIRE_FROM_FILE 5

//------------------------COIN----------------------
#define COIN_ANI 0
#define COIN_WIDTH 16
#define COIN_HEIGHT 16
//-----------------------Box---------------------
#define BOX_BBOX_WIDTH 26
#define BOX_BBOX_HEIGHT 26

#define BOX_STATE_MUSHROOM				100
#define BOX_STATE_FLOWER				200
#define BOX_STATE_STAR					300
#define BOX_STATE_FLOWER_UP				400
#define BOX_STATE_MUSHROOM_UP			500
#define BOX_STATE_STAR_UP				600


#define BOX_ANI_MUSHROOM				0
#define BOX_ANI_FLOWER					1
#define BOX_ANI_STAR					2
#define BOX_ANI_MUSHROOM_UP				3
#define BOX_ANI_FLOWER_UP				4
#define BOX_ANI_STAR_UP					5
#define BOX_SPEED_UP					0.065f

#define NUMBER_OF_ITEM_IN_BOX 3

//--------------------------EFFECT---------------------
//----BrokenBrick----
#define LOAD_EFFECT_BROKEN_BRICK		16
#define EFFECT_BROKEN_BRICK_ANI			0
#define EFFECT_BROKEN_BRICK_SPEED_X		0.02f
#define EFFECT_BROKEN_BRICK_SPEED_Y		0.05f
#define EFFECT_BROKEN_BRICK_GRAVITY		0.0009f
//----CoinEffect------
#define COIN_EFFECT_ANI					0
#define LOAD_EFFECT_COIN				17
#define TIME_EFFECT						300
#define COIN_EFFECT_SPEED_Y				0.2f
#define SET_EFFECT_IN_BRICK_X			4
#define SET_EFFECT_IN_BRICK_Y			14
#define COIN_EFFECT_MIN_Y				35
#define COIN_EFFECT_MAX_Y				10
//-------Point------------
#define POINT_EFFECT_MODEL_100			0 
#define POINT_EFFECT_MODEL_200			1	
#define	POINT_EFFECT_MODEL_400			2
#define POINT_EFFECT_MODEL_800			3 
#define	POINT_EFFECT_MODEL_1K			4 
#define	POINT_EFFECT_MODEL_2K			5
#define	POINT_EFFECT_MODEL_4K			6
#define	POINT_EFFECT_MODEL_8K			7

#define POINT_EFFECT_SPEED_Y			0.05f
#define POINT_EFFECT_MIN_Y				15
#define LOAD_POINT_EFFECT				18

#define POINT_EFFECT_ANI_100			0
#define POINT_EFFECT_ANI_200			1	
#define	POINT_EFFECT_ANI_400			2
#define POINT_EFFECT_ANI_800			3
#define	POINT_EFFECT_ANI_1K				4 
#define	POINT_EFFECT_ANI_2K				5
#define	POINT_EFFECT_ANI_4K				6
#define	POINT_EFFECT_ANI_8K				7
//-------------Disappear--------------
#define DISAPPEAR_EFFECT_ANI			0
#define LOAD_EFFECT_DISAPPEAR			14
#define TIME_EFFECT_DISAPEAR			500
//--------------Tail Hit---------------
#define TAIL_HIT_EFFECT_ANI				0
#define LOAD_EFFECT_TAIL_HIT			15
