
#ifndef __AI_H__
#define __AI_H__

#include "Common.h"
#include "Enemy.h"

// 控制敌机的产生和消失，但是不负责碰撞
class AI : public Node
{
public:

	CREATE_FUNC(AI);
	bool init();

	// 产生敌机和检测敌机是否已经飞到窗口外
	void update(float);

	void onEnter()
	{
		Node::onEnter();
		_enemys = __Array::create();
		_enemys->retain();

		_bullets = __Array::create();
		_bullets->retain();
	}

	void onExit()
	{
		Node::onExit();
		_enemys->release();
		_bullets->release();
	}

	void genSmall(float);
	void genMiddle(float);
	void genBig(float);
	void genEnemy(Enemy::TYPE type);

protected:
	friend class SceneGame;
	// 保存所有敌机的数组
	__Array* _enemys;

	// 保存所有敌机的子弹
	__Array* _bullets;
};

#endif

