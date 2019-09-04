
#include "ItemTortoise.h"

void ItemTortoise::move(float dt)
{
	moveH(dt);
	moveV(dt);
}

void ItemTortoise::moveV(float dt)
{
	if (!canMoveV(dt))
	return;

	setPositionY(getPositionY() - dt * getSpeedV());

}

bool ItemTortoise::canMoveV(float dt)
{
	//是否死亡
	if (_statu == STATUS_STOP)
		return false;

	//是否在窗口20个像素的范围内
	Vec2 pt = Vec2(getBoundingBox().getMinX(), getBoundingBox().getMinY());
	Vec2 ptInWorld = getMap()->convertToWorldSpace(pt);
	if((ptInWorld.x - winSize.width) > 20)
		return false;

	if(!Common::canMoveDown(this, getMap(), dt*getSpeedV()))
		return false;


	return true;
}

void ItemTortoise::moveH(float dt)
{
	//判断是否能移动
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
}

bool ItemTortoise::canMoveH(float dt)
{
	//乌龟是否被踩了一次 就停止
	if (_statu == STATUS_STOP)
		return false;

	//是否在窗口20个像素的范围内
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
			return bLeft;
	}

	bool bRight = Common::canMoveRight(this, getMap(), dt*getSpeedH());
		if(!bRight)
		{
			_dir = Common::DIR_LEFT;
			updateItem();
		}

	return bRight;
}

void ItemTortoise::collision(Mario* mario)
{

	if (mario->getBoundingBox().intersectsRect(this->getBoundingBox()))
	{
		if ((mario->getBoundingBox().getMinY() - getBoundingBox().getMinY()) < (getBoundingBox().size.height / 3))
		{
			mario->killed(false);
		}
		else
		{
			if (_statu == STATUS_ALIVE)
			{
				_bDead = true;
				setStatus(STATUS_STOP);
				//造成反弹效果
				mario->setSpeedUp(mario->getSpeedDown());
				mario->setSpeedDown(0);
				scheduleOnce(schedule_selector(ItemTortoise::setDead), .3f);
			}
			else if(_statu == STATUS_STOP)
			{
				_bDead = true;
				setStatus(STATUS_SPEED);
				//造成反弹效果
				mario->setSpeedUp(mario->getSpeedDown());
				mario->setSpeedDown(0);
				scheduleOnce(schedule_selector(ItemTortoise::setDead), .3f);
			}
			else if(_statu == STATUS_SPEED)
			{
				_bDead = true;
				_speedH *= 3; //加速
				MoveBy* move = MoveBy::create(3, Vec2(0, -winSize.height));
				runAction(move);
			}
		}
	}


}

void ItemTortoise::setStatus(STATUS status)
{
	_statu = status;
	updateItem();
	
}

void ItemTortoise::updateItem()
{
	if (_statu == STATUS_ALIVE)
	{
		if(_dir == Common::DIR_LEFT)
		{
			stopAllActions();
			runAction(RepeatForever::create(
							Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_tortoise_left)
							)));
		}
		else if(_dir == Common::DIR_RIGHT)
		{
			stopAllActions();
			runAction(RepeatForever::create(
				Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_tortoise_right)
				)));
		}

	}
	else if(_statu == STATUS_STOP)
	{
		stopAllActions();
		runAction(RepeatForever::create(
			Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_tortoise_dead)
			)));
	}
	else if(_statu == STATUS_SPEED)
	{
		stopAllActions();
		runAction(RepeatForever::create(
			Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_tortoise_dead)
			)));
	}
}
