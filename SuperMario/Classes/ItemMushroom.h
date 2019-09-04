
#ifndef	__ItemMushroom_H__
#define	__ItemMushroom_H__

#include "Item.h"

class ItemMushroom : public Item
{
public:
	static ItemMushroom* create(ValueMap dict)
	{
		ItemMushroom* mushroom = new ItemMushroom;
		mushroom->init();
		//添加门牌号 不然不知道这是什么
		mushroom->_type = Item_mushroom;
		mushroom->_air = false;

		mushroom->setLocalZOrder(1000);
		mushroom->_statu = STATUS_ALIVE;
		mushroom->_dir = Common::DIR_LEFT;
		mushroom->_speedH = 30;
		mushroom->_speedV = 30;
		
		//让精灵runAction //动起来
		Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_mushroom_run));
		mushroom->runAction(RepeatForever::create(animate));

		mushroom->autorelease();
		return mushroom;
	}

	void move(float dt);

	void moveH(float dt);
	void moveV(float dt);
	bool canMoveV(float dt); //垂直移动
	bool canMoveH(float dt); //水平移动
	void collision(Mario* mario);

	enum STATUS {STATUS_ALIVE, STATUS_NONE, STATUS_DEAD1, STATUS_DEAD2}; 
	STATUS _statu; //移动方向
	bool _air;//是否在天上

	CC_SYNTHESIZE(int, _speedH, SpeedH);
	CC_SYNTHESIZE(int, _speedV, SpeedV);
	void setStatus(STATUS status);
	Common::DIR _dir;

};

#endif