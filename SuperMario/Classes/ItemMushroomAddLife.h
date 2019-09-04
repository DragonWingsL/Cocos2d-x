
#ifndef	__ItemMushroomAddLife_H__
#define	__ItemMushroomAddLife_H__

#include "ItemMushroomReward.h"

class ItemMushroomAddLife : public ItemMushroomReward
{
public:
	static ItemMushroomAddLife* create(ValueMap dict)
	{
		ItemMushroomAddLife* mushroomL = new ItemMushroomAddLife;
		mushroomL->_type = Item_MushroomAddLife;
		mushroomL->_status = SLEEP;
		mushroomL->_dir = Common::DIR_LEFT;
		mushroomL->_speedH = 100;
		mushroomL->_speedV = 50;

		mushroomL->setVisible(false);
		mushroomL->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_ITEM_mushroomR_add));
		mushroomL->setLocalZOrder(1000);
		mushroomL->autorelease();
		return mushroomL;
	}

};

#endif