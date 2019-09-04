#include "AI.h"


bool AI::init()
{
	Node::init();
	scheduleUpdate();

	// �����л���ʱ�䣬���Խ���д�������ļ��������Ժ��޸���Ϸ�߼�
	schedule(schedule_selector(AI::genSmall), 1);
	schedule(schedule_selector(AI::genMiddle), 5);
	schedule(schedule_selector(AI::genBig), 30);

	return true;
}

void AI::update(float)
{
	// ���л��Ƿ��Ѿ��ɳ�����
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

	// ����ӵ��Ƿ�ɳ�����
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

	// ����ê���Ŀ����Ϊ�˷����������λ��
	e->setAnchorPoint(Vec2(0, 0));

	// Ϊ�л�����һ�����λ�ã����ֵ����һ�����ڿ�ȷ�Χ�����Ǵ��ڿ�ȼ�ȥ�ɻ���ȷ�Χ
	float x = CCRANDOM_0_1() * (winSize.width-e->getBoundingBox().size.width);
	float y = winSize.height;

	e->setPosition(Vec2(x, y));

	// ���뵽����
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