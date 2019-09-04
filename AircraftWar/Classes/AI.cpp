#include "AI.h"


bool AI::init()
{
	Node::init();
	scheduleUpdate();

	// 创建敌机的时间，可以将其写入配置文件，方便以后修改游戏逻辑
	schedule(schedule_selector(AI::genSmall), 1);
	schedule(schedule_selector(AI::genMiddle), 5);
	schedule(schedule_selector(AI::genBig), 30);

	return true;
}

void AI::update(float)
{
	// 检测敌机是否已经飞出窗外
	int count = _enemys->count();
	for (int i = count - 1; i >= 0; --i)
	{
		Enemy* e = (Enemy *)_enemys->getObjectAtIndex(i);
		if (Util::isAllOutOfWindow(e))
		{
			e->removeFromParent();
			_enemys->removeObject(e);
		}
		else
		{
			e->shoot(_bullets);
		}
	}

	// 检测子弹是否飞出窗外
	count = _bullets->count();
	for (int i = count - 1; i >= 0; --i)
	{
		Bullet* b = (Bullet *)_bullets->getObjectAtIndex(i);
		if (Util::isAllOutOfWindow(b))
		{
			b->removeFromParent();
			_bullets->removeObject(b);
		}
	}
	
}

void AI::genEnemy(Enemy::TYPE type)
{
	Enemy* e = Enemy::create(type);
	addChild(e);

	// 设置锚点的目的是为了方便设置随机位置
	e->setAnchorPoint(Vec2(0, 0));

	// 为敌机设置一个随机位置，随机值不是一个窗口宽度范围，而是窗口宽度减去飞机宽度范围
	float x = CCRANDOM_0_1() * (winSize.width-e->getBoundingBox().size.width);
	float y = winSize.height;

	e->setPosition(Vec2(x, y));

	// 加入到数组
	_enemys->addObject(e);
}

void AI::genSmall(float)
{
	genEnemy(Enemy::SMALL);
}

void AI::genMiddle(float)
{
	genEnemy(Enemy::MIDDLE);
}

void AI::genBig(float)
{
	genEnemy(Enemy::BIG);
}