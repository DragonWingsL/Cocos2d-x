#ifndef __Hero_H__
#define __Hero_H__

#include "Common.h"

class Hero : public Sprite
{
public:
	static Hero* create(int id)
	{
		Hero* h = new Hero;
		h->init(id);
		h->autorelease();
		return h;
	}
	bool init(int id);

	void update(float);

	bool canFire();
	void fire();
	void fire0();
	void fire1();
	void fire2();
	void fire3();
	void fire4();

	void fireUp(const Point& deltaStartPosition = Vec2(0, 0));
	void fireUp(int angle, const Point& delta=Vec2(0, 0));
	Sprite* genBullet();
	void checkBullets();

	__Array* _bullets;
	void onEnter()
	{
		Sprite::onEnter();
		_bullets = __Array::create();
		_bullets->retain();
	}

	void onExit()
	{
		Sprite::onExit();
		_bullets->release();
	}

	void killed()
	{}

protected:
	friend class SceneGame;
	int _damage;
	int _shootCD;
	int _curCD;
	int _hp;
	int _id;
};

#endif
