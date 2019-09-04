
#include "ItemFlag.h"
#include "Mario.h"
#include "LayerGame.h"

void ItemFlag::move(float dt)
{
	if(_bMove && !_bComplete)
	{
		//移动旗子
		setPositionY(getPositionY() - dt * _speed);
	}

}

void ItemFlag::collision(Mario* mario)
{
	if(mario->getTouchFlag())
	{
		_bMove = true;
	}
	if(_bMove)
	{
		//旗子跟马里奥碰到了 停止旗子的移动
		if(mario->getBoundingBox().intersectsRect(getBoundingBox()))
		{
			_bComplete = true;
			mario->autoRun(); //马里奥自动移动

			LayerGame* game = (LayerGame*)getParent()->getParent();
			game->destoryBridge();
		}
	}
	
	
}