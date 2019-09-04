
#include "ItemBoss.h"
#include "LayerGame.h"

void ItemBoss::move(float dt)
{
	//桥断了 掉下去 不能左右移动 //如果在跳就不进行判断
	if( !_bJump && Common::canMoveDown(this, this->getMap(), dt*_speedV))
	{
		setPositionY(getPositionY() - dt * _speedV);
		return;
	}
	
	if(_dir == Common::DIR_LEFT)
	{
		setPositionX(getPositionX() - dt*_speedH);
		
		if(getPositionX() < _left)
		{
			_body->stopAllActions();
			_body->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_boss_right))));
			_dir = Common::DIR_RIGHT;
		}
	}
	else if(_dir == Common::DIR_RIGHT)
	{
		setPositionX(getPositionX() + dt*_speedH);
		
		if(getPositionX() > _right)
		{
		_body->stopAllActions();
		_body->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_boss_left))));
			_dir = Common::DIR_LEFT;
		}
	}

	//boss jump
	int r = CCRANDOM_0_1() * 60;
	if(r == 0)
	{
		if(_bJump)
			return;

		_bJump = true;
		JumpBy* Jump = JumpBy::create(.2f, Vec2(0, 0), 20, 1);
		CallFunc* call = CallFunc::create([=]() {this->setJumpFalse(); });
		runAction(CCSequence::create(Jump, call, NULL));
	}

	//boss 发射子弹
	int bullet = CCRANDOM_0_1() * 60;
	if(bullet == 0)
	{
		LayerGame* game = (LayerGame*)getParent()->getParent();
		game->fireBullet(this);
	}
	
}