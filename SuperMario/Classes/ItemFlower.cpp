
#include "ItemFlower.h"

void ItemFlower::move(float dt)
{
	if(_statu == STATUS_UP)
	{
		if(getPositionY() > _ptStart.y)
		{
			_statu = STATUS_SHOW;
			scheduleOnce(schedule_selector(ItemFlower::setStatusDown), 2.0f);
			return;
		}
		setPositionY(getPositionY() + dt*_speed);
	}
	else if(_statu == STATUS_DOWN)
	{
		if(getPositionY() <= _ptEnd.y)
		{
			_statu = STATUS_UP;
			return;
		}
		setPositionY(getPositionY() - dt*_speed);
	}

}

void ItemFlower::collision(Mario* mario)
{
	Rect rc = getBoundingBox();
	//�ж�������Ƿ�������½�����ײ��
	if(mario->getBoundingBox().intersectsRect(rc))
	{
		mario->killed(false);
	}
	else
	{
		//�ж�������Ƿ��ڻ������� ��������滨�Ͳ��ƶ�
		rc.size.height = winSize.height;
		if (mario->getAir() == false && mario->getBoundingBox().intersectsRect(rc))
		{
			//���������ͣ�ڻ��������ʱ�� �����ܳ���
			if(_statu == STATUS_STOP) return;
			_oldStatu = _statu;
			_statu = STATUS_STOP;
		}
		else if(_statu == STATUS_STOP)
		{
			_statu = _oldStatu;
		}
	}

}