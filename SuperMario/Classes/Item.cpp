
#include "Item.h"
#include "ItemMushroom.h"
#include "ItemTortoise.h"
#include "ItemFlower.h"
#include "ItemMushroomReward.h"
#include "ItemMushroomAddLife.h"
#include "ItemFlag.h"
#include "ItemFinalPoint.h"
#include "ItemFlyFish.h"
#include "ItemTortiseRound.h"
#include "ItemLadderLR.h"
#include "ItemTortoiseFly.h"
#include "ItemLadderUD.h"
#include "ItemFireString.h"
#include "ItemBoss.h"

Item* Item::create(ValueMap dict)
{
	const std::string str = dict["type"].asString();
	Item* item = NULL;

	if(str == "mushroom")
	{
		item = ItemMushroom::create(dict);
	}
	else if(str == "tortoise")
	{
		item = ItemTortoise::create(dict);
	}
	else if(str == "flower")
	{
		item = ItemFlower::create(dict);
	}
	else if(str == "MushroomReward")
	{
		item = ItemMushroomReward::create(dict);
	}
	else if(str == "MushroomAddLife")
	{
		item = ItemMushroomAddLife::create(dict);
	}
	else if(str == "flagpoint")
	{
		item = ItemFlag::create(dict);
	}
	else if(str == "finalpoint")
	{
		item = ItemFinalPoint::create(dict);
	}
	else if(str == "flyfish")
	{
		item = ItemFlyFish::create(dict);
	}
	else if(str == "tortoise_round")
	{
		item = ItemTortoiseRound::create(dict);
	}
	else if(str == "ladderLR")
	{
		item = ItemLadderLR::create(dict);
	}
	else if(str == "tortoise_fly")
	{
		item = ItemTortoiseFly::create(dict);
	}
	else if(str == "ladderUD")
	{
		item = ItemLadderUD::create(dict);
	}
	else if(str == "fire_string")
	{
		item = ItemFireString::create(dict);
	}
	else if(str == "boss")
	{
		item = ItemBoss::create(dict);
	}


	if(item)
	{
		item->_bDead = false;

		const Value x = dict["x"];
		const Value y = dict["y"];
		item->setAnchorPoint(Vec2(0, 0));
		item->setPosition(Vec2(x.asInt(), y.asInt() - 16));
	}
	return item;
}