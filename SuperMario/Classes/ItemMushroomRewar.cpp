
#include "ItemMushroomReward.h"
#include "Mario.h"

void ItemMushroomReward::move(float dt)
{
	if(_status == SLEEP)	 return;
	if(_status == GROW) return;

	moveH(dt);
	moveV(dt);

}

void ItemMushroomReward::moveV(float dt)
{
	if (!canMoveV(dt))
		return;

	setSpeedH(10);
	setPositionY(getPositionY() - dt * getSpeedV());

}

bool ItemMushroomReward::canMoveV(float dt)
{

	//�Ƿ��ڴ���20�����صķ�Χ��
	Vec2 pt = Vec2(getBoundingBox().getMinX(), getBoundingBox().getMinY());
	Vec2 ptInWorld = getMap()->convertToWorldSpace(pt);
	if((ptInWorld.x - winSize.width) > 20)
		return false;

	if(!Common::canMoveDown(this, getMap(), dt*getSpeedV()))
		return false;


	return true;
}

void ItemMushroomReward::moveH(float dt)
{
	//�ж��Ƿ����ƶ�
	if(!canMoveH(dt))
		return;

	if (_dir == Common::DIR_LEFT)
	{
		//move mushroom left 
		setPositionX(getPositionX() - dt * getSpeedH());
	}

	if(_dir == Common::DIR_RIGHT)
	{
		//move mushroom right
		setPositionX(getPositionX() + dt * getSpeedH());
	}
	setSpeedH(30);
}

bool ItemMushroomReward::canMoveH(float dt)
{

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
			return false;}
		return bLeft;
	}

	bool bRight = Common::canMoveRight(this, getMap(), dt*getSpeedH());
	if(!bRight)
	{
		_dir = Common::DIR_LEFT;
	}

	return bRight;
}

void ItemMushroomReward::collision(Mario* mario)
{
	//Ģ�����ͺ�����µ���ײ���
	if(_status == SLEEP || _status == GROW) return;

	if(mario->getBoundingBox().intersectsRect(getBoundingBox()))
	{
		_bDead = true;
		mario->eatItem(_type);
		setVisible(false);
	}


}

void ItemMushroomReward::setStatuMove()
{
	_status = MOVE;
	setVisible(true);
}
