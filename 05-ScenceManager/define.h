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

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
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
	BOOMERANGBROTHER = 24,
	BOOMERANG = 25,
	POOP = 26,
	MUSICBRICK = 27,

	EFFECT_DISAPPEAR = 14,
	EFFECT_TAIL_HIT = 15,
	EFFECT_BROKEN_BRICK = 16,
	EFFECT_COIN = 17,
	EFFECT_POINT = 18,
	EFFECT_1UP = 19,

	MUSHROOM_SYMBOL = 20,
	FLOWER = 21,
	STAR = 22,
	BOX = 23,

	MUSHROOM_POWER = 50,
	MUSHROOM_1_UP = 51,
	PORTAL = 52,
	BRICK = 57,

//----------INTRO---------------
	GROUND = 58,
	CURTAIN = 59,
	ARROW = 60,
	SHININGTHREE = 61,
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
#define DISTANCE_MARIO_FLY_THROUGH_SKY_Y	265
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
#define OBJECT_TYPE_PLATFORM			7
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_COLOR_BLOCK			4
#define OBJECT_TYPE_FIRE_BALL			5
#define OBJECT_TYPE_FIRE_PLANT			6
#define OBJECT_TYPE_PIPE				1
#define OBJECT_TYPE_QUESTION_BRICK		8
#define OBJECT_TYPE_GOLD_BRICK			9
#define OBJECT_TYPE_COIN				10
#define OBJECT_TYPE_MUSHROOM			11
#define OBJECT_TYPE_LEAF				12
#define OBJECT_TYPE_BOX					13
#define OBJECT_TYPE_MUSIC_BRICK			14
#define OBJECT_TYPE_BOOMERANG_BROTHER	15
#define OBJECT_TYPE_PORTAL				52
#define OBJECT_TYPE_BUSH				53
#define OBJECT_TYPE_START				54
#define OBJECT_TYPE_SCENE				55
#define OBJECT_TYPE_CARD				56
#define OBJECT_TYPE_BRICK				57
#define OBJECT_TYPE_INTRO_GROUND		58
#define OBJECT_TYPE_INTRO_CURTAIN		59
#define OBJECT_TYPE_INTRO_ARROW			60
#define OBJECT_TYPE_INTRO_SHININGTHREE	61
#define GRID							999

#define MAX_SCENE_LINE					1024
#define WORLDMAP						0
#define MAP1_1							1
#define MAP1_1_1						2
#define	MAP1_3							3
#define MAP1_3_1						4
#define INTRO							5
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


//-------------------------Pipe-----------
#define SHORT_PIPE						1
#define MEDIUM_PIPE						2
#define ANI_SHORT_PIPE					0
#define ANI_MEDIUM_PIPE					1
#define PIPE_BBOX_WIDTH					32
#define PIPE_SHORT_BBOX_HEIGHT			32 
#define PIPE_MEDIUM_BBOX_HEIGHT			48

//--------------------Brick------------------------
#define BRICK_BBOX_WIDTH					16
#define BRICK_BBOX_HEIGHT					16

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

#define MUSIC_BRICK_MODEL_HIDDEN				2
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

//-----------------Mushroom
#define MUSHROOM_MODEL_RED						1
#define MUSHROOM_MODEL_GREEN					2
#define MUSHROOM_BBOX_WIDTH						16
#define MUSHROOM_BBOX_HEIGHT					16
#define MUSHROOM_RED_ANI						0
#define MUSHROOM_GREEN_ANI						1
#define MUSHROOM_GRAVITY						0.02f
#define MUSHROOM_SPEED							0.0035f
#define MUSHROOM_SPEED_APPEAR_Y					0.01f
#define MUSHROOM_STATE_APPEAR					100
#define MUSHROOM_STATE_MOVING					200
#define LOAD_MUSHROOM_FROM_TXT					12






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
