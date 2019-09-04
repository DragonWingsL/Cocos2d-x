
#include "ItemMushroom.h"

void ItemMushroom::move(float dt)
{
	moveH(dt);
	moveV(dt);
}

void ItemMushroom::moveV(float dt)
{
	if (!canMoveV(dt))
	return;

	setSpeedH(10);
	setPositionY(getPositionY() - dt * getSpeedV());

}

bool ItemMushroom::canMoveV(float dt)
{
	//是否死亡
	if (_statu == STATUS_DEAD1 || _statu == STATUS_DEAD2)
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

void ItemMushroom::moveH(float dt)
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
	setSpeedH(30);
}

bool ItemMushroom::canMoveH(float dt)
{
	//是否死亡
	if (_statu == STATUS_DEAD1 || _statu == STATUS_DEAD2)
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

void ItemMushroom::collision(Mario* mario)
{

	if (mario->getBoundingBox().intersectsRect(this->getBoundingBox()))
	{
		//只有马里奥没死才进行
		if(mario->getDead() != Mario::DEAD_NONE)
			return;

		if ((mario->getBoundingBox().getMinY() - getBoundingBox().getMinY()) < (getBoundingBox().size.height / 3))
		{
			
			mario->killed(false);
		}
		else
		{
			_bDead = true;
			setStatus(STATUS_DEAD1);
		}
	}


}

void ItemMushroom::setStatus(STATUS status)
{
	stopAllActions();
	_statu = status;
	if (_statu == STATUS_DEAD1)
	{
		//播放dead1 的动画
		setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_ITEM_mushroom_dead1));
		MoveBy* move = MoveBy::create(3, Vec2(0, -winSize.height));
		runAction(move);
	}
}
