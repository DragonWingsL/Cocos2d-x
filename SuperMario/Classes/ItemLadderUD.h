
#ifndef	__ItemLadderUD_H__
#define __ItemLadderUD_H__

#include "Item.h"

class ItemLadderUD : public Item
{
public:
	static ItemLadderUD* create(ValueMap dict)
	{
		ItemLadderUD* ladder = new ItemLadderUD;

		ladder->_roundDis = dict["ladderDis"].asInt();
		ladder->_UorD = dict["UorD"].asInt();

		ladder->initWithFile("ladder.png");
		ladder->autorelease();
		return ladder;
	}

	void onEnter()
	{
		Item::onEnter();
		_UorD = 1 - _UorD;

		//һ��ʼ�ƶ��ķ����������ƶ�
		if(_UorD == 1)
		{
			_up = getPositionY() - _roundDis;
			_down = getPositionY() - 8;
		}
		else
		{
			_up = getPositionY() - 8;
			_down = getPositionY() + _roundDis;
		}

		_speed = 50;
		_mario = NULL;
	}

	void move(float dt);
	void collision(Mario* mario);

	int _roundDis; 
	int _up;
	int _down;
	int _UorD; //һ��ʼ�ƶ��ķ���
	int _speed; //�ƶ����ٶ�
	Mario* _mario;

};

#endif