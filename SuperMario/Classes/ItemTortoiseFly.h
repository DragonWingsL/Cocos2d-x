
#ifndef __ItemTortoiseFly_H__
#define __ItemTortoiseFly_H__

#include "Item.h"

class ItemTortoiseFly : public Item
{
public:
	static ItemTortoiseFly* create(ValueMap dict)
	{
		ItemTortoiseFly* tortoiseFly = new ItemTortoiseFly;
		tortoiseFly->_type = Item_tortoise_fly;
		tortoiseFly->_flyDis = dict["flyDis"].asInt();
		tortoiseFly->init();
		tortoiseFly->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_tortoise_flyLeft))));
		tortoiseFly->autorelease();
		return tortoiseFly;
	}

	void onEnter()
	{
		Item::onEnter();

		_top = getPositionY();
		_down = getPositionY() - _flyDis;
		_dir = Common::DIR_UP;
		_speed = 50;
	}

	void move(float dt);
	void collision(Mario* mario);

	int _top;
	int _down;
	int _flyDis;
	Common::DIR _dir;
	int _speed;
};
#endif