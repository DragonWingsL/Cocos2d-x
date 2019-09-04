
#include "ItemLadderLR.h"
#include "Mario.h"

void ItemLadderLR::move(float dt)
{
	if(_LorR == 1)
	{
		setPositionX(getPositionX() - dt*_speed);
		if (_mario) _mario->setPositionX(_mario->getPositionX() - dt * _speed);
		if(_left > getPositionX())
			_LorR = 1 - _LorR; //ȡ������
	}
	else 
	{
		setPositionX(getPositionX() + dt*_speed);
		if (_mario) _mario->setPositionX(_mario->getPositionX() + dt * _speed);
		if(_right < getPositionX())
			_LorR = 1 - _LorR;
	}

}

void ItemLadderLR::collision(Mario* mario)
{
	//����º�ladder��������ײ
		//������ڷɵ�ʱ��������
		if(mario->getSpeedUp() > 0)
		{
			if(mario->getBoundingBox().intersectsRect(getBoundingBox()))
			{
				mario->setSpeedDown(mario->getSpeedUp());
				mario->setSpeedUp(0);
				mario->setAir(false);
				mario->updateMario();
			}
		}
		else 
		{
			if(mario->getBoundingBox().intersectsRect(getBoundingBox()))
			{
				mario->setSpeedDown(mario->getGravity());
				mario->setAir(false);
				mario->updateMario();
				mario->_onLadder = true;
				_mario = mario; //�������ladder �����ʱ�򱣴� Ȼ��ladder�ƶ�����¸����ƶ�				
			}
			else 
			{
				if(_mario)
				{
					mario->_onLadder = false;
					_mario = NULL;
				}
				
			}
		}

}