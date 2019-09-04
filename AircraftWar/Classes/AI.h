
#ifndef __AI_H__
#define __AI_H__

#include "Common.h"
#include "Enemy.h"

// ���Ƶл��Ĳ�������ʧ�����ǲ�������ײ
class AI : public Node
{
public:

	CREATE_FUNC(AI);
	bool init();

	// �����л��ͼ��л��Ƿ��Ѿ��ɵ�������
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
	// �������ел�������
	__Array* _enemys;

	// �������ел����ӵ�
	__Array* _bullets;
};

#endif

