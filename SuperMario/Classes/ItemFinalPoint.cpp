
#include "ItemFinalPoint.h"
#include "Mario.h"
#include "LayerGame.h"

void ItemFinalPoint::move(float dt)
{

}

void ItemFinalPoint::collision(Mario* mario)
{
	//�������������� ��ͨ��
	//������һ��
	if(mario->getBoundingBox().intersectsRect(getBoundingBox()))
	{
		LayerGame* game = (LayerGame*)getParent()->getParent();
		game->victory();
	}
}