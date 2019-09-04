
#ifndef	__ItemFlag_H__
#define	__ItemFlag_H__

#include "Item.h"

class ItemFlag : public Item
{
public:
	static ItemFlag* create(ValueMap dict)
	{
		ItemFlag* flag = new ItemFlag;
		flag->initWithFile("flag.png");

		flag->_bMove = false;
		flag->_bComplete = false;
		flag->_speed = 100;
		flag->autorelease();
		return flag;
	}

	void onEnter()
	{
		Item::onEnter();
		//旗子的位置不对要进行偏移 
		setPositionX(getPositionX() - 16);
	}

	void move(float dt);
	void collision(Mario* mario);
	bool _bMove; //是否移动旗子
	bool _bComplete; //旗子碰到马里奥就停
	int _speed;

};
#endif