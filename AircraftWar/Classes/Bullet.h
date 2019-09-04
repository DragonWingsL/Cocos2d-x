

#ifndef __Bullet_H__
#define __Bullet_H__

#include "Common.h"

class Bullet : public Sprite
{
public:
	
	static Bullet* create(int damage)
	{
		Bullet* bullet = new Bullet;
		bullet->init(damage);
		bullet->autorelease();
		return bullet;
	}

	bool init(int damage)
	{
		Sprite::initWithFile(IMAGE_Bullet);
		_damage = damage;
		return true;
	}
protected:
	friend class SceneGame;
	int _damage;
};

#endif
