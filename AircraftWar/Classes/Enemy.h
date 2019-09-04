

// Enemy是封装敌人飞机的类

#ifndef __Enemy_H__
#define __Enemy_H__

#include "Common.h"
#include "Bullet.h"

class Enemy : public Sprite
{
public:
	enum TYPE{ SMALL, MIDDLE, BIG };

	static Enemy* create(TYPE type)
	{
		Enemy* enemy = new Enemy;
		enemy->init(type);
		enemy->autorelease();
		return enemy;
	}

	bool init(TYPE type)
	{
		Sprite::initWithFile(Util::format(type + 1, "Enemy", ".png"));

		_type = type;

		int hp[] = { 10, 50, 200 };
		_hp = hp[type];

		// 设置小飞机256是让小飞机在4秒内从窗口上方落下
		int speed[] = { 256, 128, 50 };
		_speed = speed[type];

		int damage[] = { 20, 40, 100 };
		_damage = damage[type];

		_curCD = 0;
		int shootCD[] = { 60, 120, 250 };
		_shootCD = shootCD[type];

		int score[] = { 1000, 3000, 10000 };
		_score = score[type];

		// 运动敌机
		scheduleUpdate();

		return true;
	}

	void update(float dt)
	{
		setPositionY( getPositionY() - dt*_speed );
	}

	bool canFire()
	{
		_curCD++;
		if (_curCD != _shootCD)
			return false;
		_curCD = 0;
		return true;
	}

	void fire(__Array* bullets)
	{
		Bullet* b = Bullet::create(_damage);
		bullets->addObject(b);
		getParent()->addChild(b);

		// 子弹位置要偏移的原因是因为飞机的锚点在Vec2(0, 0)
		float off = this->getBoundingBox().size.width / 2;
		b->setPosition(this->getPosition()+Vec2(off, 0));
		b->runAction(
			MoveBy::create(winSize.height/(_speed * 2), Vec2(0, -winSize.height)));
	}
	
	// 敌机发射子弹的接口
	void shoot(__Array* bullets)
	{
		// 如果敌机已经死亡，就不用再发射子弹了
		if (_hp <= 0) return;

		if (canFire())
			fire(bullets);
	}

	void killed()
	{
		// 执行爆炸动画，然后删除对象
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(ANI_PLIST_PFBoom);

		// __Array* frames = __Array::create();
		Vector<SpriteFrame *> frames;
		for (int i = 0; i < 18; ++i)
		{
			char* frameName = Util::format(i + 1, "Boom_", ".png");
			SpriteFrame* frame = cache->getSpriteFrameByName(frameName);
			frames.pushBack(frame);
		}

		Animation* animation = Animation::createWithSpriteFrames(frames, .05f);
		Animate* animate = Animate::create(animation);
		RemoveSelf* remove = RemoveSelf::create();

		this->runAction(Sequence::create(animate, remove, NULL));
		
	}

protected:
	friend class SceneGame;
	int _hp;
	TYPE _type;
	int _speed;
	int _damage;
	int _shootCD;
	int _curCD;
	int _score;
};

#endif
