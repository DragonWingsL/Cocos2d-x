
#include "ItemTortiseRound.h"

bool ItemTortoiseRound::canMoveH(float dt)
{
	//�ڹ��Ƿ񱻲���һ�� ��ֹͣ
	if (_statu == STATUS_STOP)
		return false;

	//�Ƿ��ڴ���20�����صķ�Χ��
	Vec2 pt = Vec2(getBoundingBox().getMinX(), getBoundingBox().getMinY());
	Vec2 ptInWorld = getMap()->convertToWorldSpace(pt);
	if((ptInWorld.x - winSize.width) > 20)
		return false;

	if (_dir == Common::DIR_LEFT)
	{
		bool bLeft = Common::canMoveLeft(this, getMap(), dt*getSpeedH());
		if(!bLeft)
		{
			_dir = Common::DIR_RIGHT;
			updateItem();
		}

		if(bLeft && getPositionX() < _left)
		{
			//����������Ƶľ��� ��������
			bLeft = false;
			_dir = Common::DIR_RIGHT;
			updateItem();
		}

		return bLeft;
	}

	bool bRight = Common::canMoveRight(this, getMap(), dt*getSpeedH());
	if(!bRight)
	{
		_dir = Common::DIR_LEFT;
		updateItem();
	}

	if(bRight && getPositionX() > _right)
	{
		//����������Ƶľ��� ��������
		bRight = false;
		_dir = Common::DIR_LEFT;
		updateItem();
	}

	return bRight;
}