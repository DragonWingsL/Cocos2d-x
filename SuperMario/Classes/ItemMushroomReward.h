
#ifndef	__ItemMushroomReward_H__
#define	__ItemMushroomReward_H__

#include "Item.h"

class ItemMushroomReward : public Item
{
public:
	enum STATUS{SLEEP, GROW, MOVE};
	static ItemMushroomReward* create(ValueMap dict)
	{
		ItemMushroomReward* mushroomR = new ItemMushroomReward;
		mushroomR->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_ITEM_mushroomR_big));

		mushroomR->setLocalZOrder(1000);
		mushroomR->_status = SLEEP;
		mushroomR->_type = Item_MushroomReward;
		mushroomR->_dir = Common::DIR_LEFT;
		mushroomR->_speedH = 100;
		mushroomR->_speedV = 50;

		mushroomR->setVisible(false);
		mushroomR->autorelease();
		return mushroomR;
	}

	void move(float dt);
	void collision(Mario* mario);
	void setStatuMove();
	bool canMoveH(float);
	bool canMoveV(float);
	void moveH(float);
	void moveV(float);

	CC_SYNTHESIZE(int, _speedH, SpeedH);
	CC_SYNTHESIZE(int, _speedV, SpeedV);

	STATUS _status;
	Common::DIR _dir;

};
#endif