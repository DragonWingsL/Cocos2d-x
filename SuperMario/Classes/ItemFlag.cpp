
#include "ItemFlag.h"
#include "Mario.h"
#include "LayerGame.h"

void ItemFlag::move(float dt)
{
	if(_bMove && !_bComplete)
	{
		//�ƶ�����
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
		//���Ӹ������������ ֹͣ���ӵ��ƶ�
		if(mario->getBoundingBox().intersectsRect(getBoundingBox()))
		{
			_bComplete = true;
			mario->autoRun(); //������Զ��ƶ�

			LayerGame* game = (LayerGame*)getParent()->getParent();
			game->destoryBridge();
		}
	}
	
	
}