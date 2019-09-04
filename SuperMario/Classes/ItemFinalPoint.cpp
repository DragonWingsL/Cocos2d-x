
#include "ItemFinalPoint.h"
#include "Mario.h"
#include "LayerGame.h"

void ItemFinalPoint::move(float dt)
{

}

void ItemFinalPoint::collision(Mario* mario)
{
	//如果跟马里奥碰到 就通关
	//进入下一关
	if(mario->getBoundingBox().intersectsRect(getBoundingBox()))
	{
		LayerGame* game = (LayerGame*)getParent()->getParent();
		game->victory();
	}
}