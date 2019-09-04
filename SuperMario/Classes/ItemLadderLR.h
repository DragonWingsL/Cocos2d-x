
#ifndef	__ItemLadderLR_H__
#define __ItemLadderLR_H__

#include "Item.h"

class ItemLadderLR :public Item
{
public:
	static ItemLadderLR* create(ValueMap dict)
	{
		ItemLadderLR* ladder = new ItemLadderLR;

		ladder->_roundDis = dict["ladderDis"].asInt();
		ladder->_LorR = dict["LorR"].asInt();

		ladder->initWithFile("ladder.png");
		ladder->autorelease();
		return ladder;
	}

	void onEnter()
	{
		Item::onEnter();
		setAnchorPoint(Vec2(.5f, .5f)); //移动减少误差
		_LorR = 1 - _LorR;

		//一开始移动的方向来决定移动
		if(_LorR == 1)
		{
			_left = getPositionX() - _roundDis;
			_right = getPositionX();
		}
		else
		{
			_left = getPositionX();
			_right = getPositionX() + _roundDis;
		}
		
		_speed = 50;
		_mario = NULL;
	}

	void move(float dt);
	void collision(Mario* mario);

	int _roundDis; 
	int _left;
	int _right;
	int _LorR; //一开始移动的方向
	int _speed; //移动的速度
	Mario* _mario;

};

#endif