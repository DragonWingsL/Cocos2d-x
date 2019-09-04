
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
		//������ƺ� ��Ȼ��֪������ʲô
		tortoise->_type = Item_tortoise;
		tortoise->_air = false;

		tortoise->setLocalZOrder(1000);
		tortoise->_statu = STATUS_ALIVE;
		tortoise->_dir = Common::DIR_LEFT;
		tortoise->_speedH = 30;
		tortoise->_speedV = 30;
		
#if 0
		//�þ���runAction //������
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
	bool canMoveV(float dt); //��ֱ�ƶ�
	virtual bool canMoveH(float dt); //ˮƽ�ƶ�
	void collision(Mario* mario);
	void updateItem();
	void setDead(float){_bDead = false;}

	enum STATUS {STATUS_ALIVE, STATUS_NONE, STATUS_STOP, STATUS_SPEED }; 
	STATUS _statu; //�ƶ�����
	bool _air;//�Ƿ�������

	CC_SYNTHESIZE(int, _speedH, SpeedH);
	CC_SYNTHESIZE(int, _speedV, SpeedV);
	void setStatus(STATUS status);
	Common::DIR _dir;

};

#endif