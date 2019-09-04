
#ifndef	__Macro_H__
#define	__Macro_H__

//game level 
#define GAME_LEVEL 8

//background
#define IMAGE_MENU_background				 "background.png"
#define IMAGE_START_bg							"bg.png"

//menu button
#define IMAGE_MENU_start_1			 "startgame_normal.png"
#define IMAGE_MENU_start_2			 "startgame_select.png"
#define IMAGE_MENU_setting_1 		   "Setting_n.png"
#define IMAGE_MENU_setting_2		   "setting_s.png"
#define IMAGE_MENU_quit_1 			   "quitgame_normal.png"
#define IMAGE_MENU_quit_2		 	   "quitgame_select.png"
#define IMAGE_MENU_about_1			"about_normal.png"
#define IMAGE_MENU_about_2			"about_select.png"

//back
#define IMAGE_START_back_1				"backA.png"
#define IMAGE_START_back_2				"backB.png"

//control
#define IMAGE_CONTROLS_UI								"controlUI.png"
#define IMAGE_CONTROLS_UI_NONE					"backKeyImage.png"
#define IMAGE_CONTROLS_UI_LEFT					"backKeyLeft.png"
#define IMAGE_CONTROLS_UI_RIGHT					"backKeyRight.png"
#define IMAGE_CONTROLS_UI_JUMP_NORMAL	  "AB_normal.png"
#define IMAGE_CONTROLS_UI_JUMP_SELECT		  "AB_select.png"

//Mario
#define IMAGE_Mario_smallWalkRight	"smallWalkRight.png"
#define IMAGE_Mario_smallWalkLeft	"smallWalkLeft.png"
#define IMAGE_Mario_bigWalkRight		"walkRight.png"
#define IMAGE_Mario_bigWalkLeft		"walkLeft.png"
#define IMAGE_Mario_smallDie			"small_die.png"

//MarioFrameName
//#define FRAME_MarioName					"smallWalkRight_"
#define FRAME_CONTROLS_UI_JUMP_NORMAL					"FRAME_CONTROLS_UI_JUMP_NORMAL"
#define FRAME_CONTROLS_UI_JUMP_SELECT						"FRAME_CONTROLS_UI_JUMP_SELECT"
#define FRAME_Mario_smallFlyRight										"FRAME_Mario_smallFlyRight"
#define FRAME_Mario_smallFlyLeft										"FRAME_Mario_smallFlyLeft"
#define FRAME_Mario_bigFlyRight										"FRAME_Mario_bigFlyRight"
#define FRAME_Mario_bigFlyLeft											"FRAME_Mario_bigFlyLeft"

//Item FRAME
//mushroom frame
#define FRAME_ITEM_mushroom_dead1						"FRAME_ITEM_mushroom_dead1"
#define FRAME_ITEM_mushroom_dead2						"FRAME_ITEM_mushroom_dead2"
//mushroomReward frame
#define FRAME_ITEM_mushroomR_big							 "FRAME_ITEM_mushroomR_big"
#define FRAME_ITEM_mushroomR_add						 "FRAME_ITEM_mushroomR_add"
#define FRAME_ITEM_mushroomR_god						 "FRAME_ITEM_mushroomR_god"

//ANI
//Mario ANI
#define ANI_Mario_smallWalkRight								"ANI_Mario_smallWalkRight"
#define ANI_Mario_smallWalkLeft									"ANI_Mario_smallWalkLeft"
#define ANI_Mario_bigWalkRight									"ANI_Mario_bigWalkRight"
#define ANI_Mario_bigWalkLeft									"ANI_Mario_biglWalkLeft"
#define ANI_Mario_smallDie											"ANI_Mario_smallDie"

//item ANI
#define ANI_ITEM_mushroom_run		"ANI_ITEM_mushroom_run"

//tortoise ANI
#define ANI_ITEM_tortoise_left									"ANI_ITEM_tortoise_left"
#define ANI_ITEM_tortoise_right								"ANI_ITEM_tortoise_right"
#define ANI_ITEM_tortoise_dead								"ANI_ITEM_tortoise_dead"
//flower ANI
#define ANI_ITEM_flower											"ANI_ITEM_flower"
//flyfish ANI
#define ANI_ITEM_fish_Left										"ANI_ITEM_fish_Left"
#define ANI_ITEM_fish_Right									"ANI_ITEM_fish_Right"
//fly tortoise ANI
#define ANI_ITEM_tortoise_flyLeft							ANI_ITEM_tortoise_left
#define ANI_ITEM_tortoise_flyRight							ANI_ITEM_tortoise_right
//boss ANI
#define ANI_ITEM_boss_left										"ANI_ITEM_boss_left"
#define ANI_ITEM_boss_right									"ANI_ITEM_boss_right"
//boss ANI
#define ANI_ITEM_boss_bullet_left							"ANI_ITEM_boss_bullet_left"
#define	ANI_ITEM_boss_bullet_right							"ANI_ITEM_boss_bullet_right"

//Item IMAGE
#define IMAGE_ITEM_mushroom						"Mushroom0.png"
#define IMAGE_ITEM_tortoise							"tortoise0.png"
#define IMAGE_ITEM_flower								"flower0.png"
#define IMAGE_ITEM_mushroomReward			"rewardMushroomSet.png"
#define IMAGE_ITEM_flyfish_Left						"flyFishLeft.png"
#define IMAGE_ITEM_flyfish_Right					"flyFishRight.png"
#define IMAGE_ITEM_boss								"boss.png"
#define IMAGE_ITEM_boss_bullet						"bossBullet.png"


//ITEM TYPE STATUS
enum Item_Type {Item_mushroom, Item_tortoise, Item_flower, Item_MushroomReward, Item_MushroomAddLife, Item_flagpoint, Item_finalpoint,
							Item_flyfish, Item_tortoise_round, Item_ladderLR, 
							Item_tortoise_fly, Item_ladderUD,
							Item_fire_string, Item_boss, Item_bridgestartpos, 
							Item_addmushroom, Item_darkcloud,
							Item_battery, Item_bullet
							};

/*//level 1
mushroom
tortoise
flower
MushroomReward
MushroomAddLife
MushroomReward
flagpoint
finalpoint

//level 2
flyfish
tortoise_round
ladderLR

//level 3
tortoise_fly
ladderUD

//level 4
fire_string
boss
bridgestartpos

//level 5
addmushroom
darkcloud

//level 7
battery*/

#endif