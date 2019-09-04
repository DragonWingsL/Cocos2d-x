
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
		//���ӵ�λ�ò���Ҫ����ƫ�� 
		setPositionX(getPositionX() - 16);
	}

	void move(float dt);
	void collision(Mario* mario);
	bool _bMove; //�Ƿ��ƶ�����
	bool _bComplete; //������������¾�ͣ
	int _speed;

};
#endif