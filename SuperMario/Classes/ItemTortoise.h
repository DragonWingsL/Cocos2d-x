
#ifndef	__ItemTortoise_H__
#define	__ItemTortoise_H__

#include "Item.h"

class ItemTortoise : public Item
{
public:
	static ItemTortoise* create(ValueMap dict)
	{
		ItemTortoise* tortoise = new ItemTortoise;
		tortoise->init();
		//添加门牌号 不然不知道这是什么
		tortoise->_type = Item_tortoise;
		tortoise->_air = false;

		tortoise->setLocalZOrder(1000);
		tortoise->_statu = STATUS_ALIVE;
		tortoise->_dir = Common::DIR_LEFT;
		tortoise->_speedH = 30;
		tortoise->_speedV = 30;
		
#if 0
		//让精灵runAction //动起来
		Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation());
		tortoise->runAction(RepeatForever::create(animate));
#endif
		tortoise->updateItem();

		tortoise->autorelease();
		return tortoise;
	}

	void move(float dt);

	void moveH(float dt);
	void moveV(float dt);
	bool canMoveV(float dt); //垂直移动
	virtual bool canMoveH(float dt); //水平移动
	void collision(Mario* mario);
	void updateItem();
	void setDead(float){_bDead = false;}

	enum STATUS {STATUS_ALIVE, STATUS_NONE, STATUS_STOP, STATUS_SPEED }; 
	STATUS _statu; //移动方向
	bool _air;//是否在天上

	CC_SYNTHESIZE(int, _speedH, SpeedH);
	CC_SYNTHESIZE(int, _speedV, SpeedV);
	void setStatus(STATUS status);
	Common::DIR _dir;

};

#endif