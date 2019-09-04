
#ifndef __ItemBullet_H__
#define __ItemBullet_H__

#include "ItemBoss.h"

class ItemBullet : public Item
{
public:
	static ItemBullet* create(ItemBoss* boss)
	{
		ItemBullet* bullet = new ItemBullet;
		bullet->init();
		bullet->autorelease();
		
		bullet->_dir = boss->_dir;
		bullet->_speed = 150;

		bullet->_type = Item_bullet;
		bullet->_bDead = false;

		Rect rc = boss->getBoundingBox();
		if(bullet->_dir == Common::DIR_LEFT)
		{
			bullet->setPosition(Vec2(rc.getMinX(), rc.getMaxY() - boss->getContentSize().height* .2f));
		}
		else
		{
			bullet->setPosition(Vec2(rc.getMaxX(), rc.getMaxY() - boss->getContentSize().height * .2f));
		}
		bullet->setLocalZOrder(1000);
		return bullet;
	}

	void onEnter()
	{
		Item::onEnter();
		if(_dir == Common::DIR_LEFT)
		{
			runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_boss_bullet_left))));			
		}
		else
		{
			runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation(ANI_ITEM_boss_bullet_right))));
		}
	}

	void move(float dt);
	void collision(Mario* mario);

	Common::DIR _dir;
	int _speed;
};

#endif