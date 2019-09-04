
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
		//������ƺ� ��Ȼ��֪������ʲô
		tortoise->_type = Item_tortoise_round;
		tortoise->_air = false;

		tortoise->setLocalZOrder(1000);
		tortoise->_statu = STATUS_ALIVE;
		tortoise->_dir = Common::DIR_LEFT;
		tortoise->_speedH = 30;
		tortoise->_speedV = 30;

		tortoise->_roundDis = dict["roundDis"].asInt();
		//updateItem �����action
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

	int _roundDis; //���ƾ���
	int _left; //����ߴӾ���
	int _right; //���ұߵľ���

};
#endif