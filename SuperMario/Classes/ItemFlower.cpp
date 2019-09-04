
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
	//判断马里奥是否与马里奥进行碰撞了
	if(mario->getBoundingBox().intersectsRect(rc))
	{
		mario->killed(false);
	}
	else
	{
		//判断马里奥是否在花的上面 如果在上面花就不移动
		rc.size.height = winSize.height;
		if (mario->getAir() == false && mario->getBoundingBox().intersectsRect(rc))
		{
			//当马里奥在停在花的上面的时候 花不能出来
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