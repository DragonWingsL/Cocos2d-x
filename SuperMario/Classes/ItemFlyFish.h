
#ifndef __ItemFlyFish_H__
#define	__ItemFlyFish_H__

#include "Item.h"

class ItemFlyFish : public Item 
{
public:
	enum STATUS{STOP, FLY_LEFT, FLY_RIGHT};
	static ItemFlyFish* create(ValueMap dict)
	{
		ItemFlyFish* fish = new ItemFlyFish;
		fish->init();

		Value value = dict["duration"];
		fish->_duration = value.asInt();

		value = dict["offsetH"];
		fish->_offsetH = value.asInt();

		value = dict["offsetV"];
		fish->_offsetV = value.asInt();

		fish->_status = STOP;
		fish->autorelease();
		return fish;
	}

	void onEnter()
	{
		Item::onEnter();
		_body = Sprite::create();
		_body->setPosition(Vec2(0, 0));
		addChild(_body);
	}


	void move(float);
	void collision(Mario* mario);

	//向上飞
	void raise();
	//向下飞
	void full();
	void updateItem()
	{
		_body->stopAllActions();
		if(_status == FLY_LEFT)
		{
			_body->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_fish_Left)));
		}
		else
		{
			_body->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_fish_Right)));
		}
	}

	int _duration;
	int _offsetH;
	int _offsetV;
	STATUS _status;
	Sprite* _body;
};
#endif