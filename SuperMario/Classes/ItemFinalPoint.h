
#ifndef __ItemFinalPoint_H__
#define __ItemFinalPoint_H__

#include "Item.h"

class ItemFinalPoint : public Item
{
public:
	static ItemFinalPoint* create(ValueMap dict)
	{
		ItemFinalPoint* finalPoint = new ItemFinalPoint;
		finalPoint->init();
		finalPoint->_type = Item_finalpoint;
		finalPoint->autorelease();
		return finalPoint;
	}

	//给一个大小进行碰撞检测
	void onEnter()
	{
		Item::onEnter();
		setContentSize(Size(16, 32));
	}

	void move(float dt);
	void collision(Mario* mario);
};
#endif