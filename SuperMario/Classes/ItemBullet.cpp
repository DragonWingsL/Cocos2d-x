
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
//		_bDead = true;//�ӵ���ʧ ���ڽ�����ײ���
		mario->killed(false);
	}
}