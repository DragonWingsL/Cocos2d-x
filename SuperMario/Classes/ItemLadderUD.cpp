
#include "ItemLadderUD.h"
#include "Mario.h"

void ItemLadderUD::move(float dt)
{
	if(_UorD == 1)
	{
		setPositionY(getPositionY() - dt*_speed);
		if (_mario) _mario->setPositionY(_mario->getPositionY() - dt * _speed);
		if(_up > getPositionY())
			_UorD = 1 - _UorD; //取反方向
	}
	else 
	{
		setPositionY(getPositionY() + dt*_speed);
		if (_mario) _mario->setPositionY(_mario->getPositionY() + dt * _speed);
		if(_down < getPositionY())
			_UorD = 1 - _UorD;
	}

}

void ItemLadderUD::collision(Mario* mario)
{
	//马里奥和ladder进行了碰撞
	//马里奥在飞的时候碰到了
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
			_mario = mario; //马里奥在ladder 上面的时候保存 然后ladder移动马里奥跟着移动				
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