
#include "ItemBullet.h"

void ItemBullet::move(float dt)
{
	if(_dir == Common::DIR_LEFT)
	{
		setPositionX(getPositionX() - dt*_speed);
	}
	else
	{
		setPositionX(getPositionX() + dt*_speed);
	}

}

void ItemBullet::collision(Mario* mario)
{
	if(getBoundingBox().intersectsRect(mario->getBoundingBox()))
	{
//		_bDead = true;//子弹消失 不在进行碰撞检测
		mario->killed(false);
	}
}