
#ifndef __ItemFireString_H__
#define	__ItemFireString_H__

#include "Item.h"

class ItemFireString : public Item
{
public:
	enum STATUS {STOP, MOVE};
	static ItemFireString* create(ValueMap dict)
	{
		ItemFireString* fireString = new ItemFireString;
		fireString->initWithFile("bossBullet.png");
		fireString->autorelease();
		
		fireString->_type = Item_fire_string;
		fireString->_begAngle = dict["begAngle"].asInt();
		fireString->_time = dict["time"].asInt();
		fireString->setLocalZOrder(1000);
		return fireString;
	}

	void onEnter()
	{
		Item::onEnter();
		setAnchorPoint(Vec2(0, .5f));
		setScale(.5f);
		setRotation(_begAngle);
		_status = STOP;
	}

	void move(float dt);
	void collision(Mario* mario);

	int _begAngle; //初始角度
	int _time; //旋转一周的时间
	STATUS _status;
};
#endif