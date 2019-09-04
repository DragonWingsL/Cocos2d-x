
#ifndef	__ItemFlower_H__
#define	__ItemFlower_H__

#include "Item.h"

class ItemFlower : public Item
{
public:
	enum STATUS{STATUS_UP, STATUS_DOWN, STATUS_STOP, STATUS_SHOW, STATUS_DEAD};
	static ItemFlower* create(ValueMap dict)
	{
		ItemFlower* flower = new ItemFlower;
		flower->init();

		flower->_type = Item_flower;
		flower->_statu = STATUS_UP;
		flower->_speed = 50;

		
		//runAction()
		flower->runAction(RepeatForever::create(
										Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_flower)
										)));

		

	//	flower->setLocalZOrder(1000);
		flower->autorelease();
		return flower;
	}

	void onEnter()
	{
		Item::onEnter();

		setPositionX(getPositionX() - 8);
		_ptStart = getPosition();
		_ptEnd = Vec2(getPositionX(), 32);
	}

	void move(float );
	void collision(Mario* mario);
	void setStatusDown(float){_statu = STATUS_DOWN;}

	Vec2 _ptStart;
	Vec2 _ptEnd;

	STATUS _statu;
	int _speed;
	STATUS _oldStatu;

};
#endif