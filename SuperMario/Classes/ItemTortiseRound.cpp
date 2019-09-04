
#include "ItemTortiseRound.h"

bool ItemTortoiseRound::canMoveH(float dt)
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

		if(bLeft && getPositionX() < _left)
		{
			//如果超过回绕的距离 就往回绕
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
		//如果超过回绕的距离 就往回绕
		bRight = false;
		_dir = Common::DIR_LEFT;
		updateItem();
	}

	return bRight;
}