
#ifndef _ItemBoss_H__
#define _ItemBoss_H__

#include "Item.h"

class ItemBoss : public Item
{
public:
	static ItemBoss* create(ValueMap dict)
	{
		ItemBoss* boss =new ItemBoss;
		boss->init();
		boss->autorelease();

		boss->_speedH = 100;
		boss->_speedV = 50;
		boss->_dir = Common::DIR_LEFT;

		return boss;
	}

	void onEnter()
	{
		Item::onEnter();
	//	setPositionY(getPositionY() + 16);
		_right = getPositionX();
		_left = getPositionX() - 120;
		_body = Sprite::create();
		_body->setAnchorPoint(Vec2(0, 0));
		_body->setPosition(Vec2(0, 0));
		addChild(_body);
		_bJump = false;
		setContentSize(Size(32, 32));
	}

	void move(float dt);
	void collision(Mario* mario){}

	void setJumpFalse() {_bJump = false;}

	Sprite* _body;
	Common::DIR _dir;
	int _speedH;
	int _speedV;
	int _left;
	int _right;
	bool _bJump;

};
#endif