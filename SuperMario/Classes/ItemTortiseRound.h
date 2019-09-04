
#ifndef __ItemTortoiseRound_H__
#define __ItemTortoiseRound_H__

#include "ItemTortoise.h"

class ItemTortoiseRound : public ItemTortoise
{
public:
	static ItemTortoiseRound* create(ValueMap dict)
	{
		ItemTortoiseRound* tortoise = new ItemTortoiseRound;
		tortoise->init();
		//添加门牌号 不然不知道这是什么
		tortoise->_type = Item_tortoise_round;
		tortoise->_air = false;

		tortoise->setLocalZOrder(1000);
		tortoise->_statu = STATUS_ALIVE;
		tortoise->_dir = Common::DIR_LEFT;
		tortoise->_speedH = 30;
		tortoise->_speedV = 30;

		tortoise->_roundDis = dict["roundDis"].asInt();
		//updateItem 里进行action
		tortoise->updateItem();

		tortoise->autorelease();
		return tortoise;
	}

	void onEnter()
	{
		ItemTortoise::onEnter();

		_right = getPositionX();
		_left = getPositionX() - _roundDis;
	}

	bool canMoveH(float dt);

	int _roundDis; //回绕距离
	int _left; //最左边从距离
	int _right; //最右边的距离

};
#endif