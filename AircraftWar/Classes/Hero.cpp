#include "Hero.h"

bool Hero::init(int id)
{
	// 保存飞机的ID，用于发射子弹，不同的飞机发射子弹的方式不一样
	_id = id;

	// 获取这个飞机的基础信息，包括damage，hp，产生子弹的时间间隔，还有图片纹理
	__Dictionary* dict = __Dictionary::createWithContentsOfFile(CFG_planeinfo);

	// 战斗机的战斗力
	const __String* value = dict->valueForKey(Util::format(id + 1, "planedamageamount"));
	_damage = value->intValue();

	// 战斗机的发射子弹的时间间隔
	value = dict->valueForKey(Util::format(id + 1, "planeshootspeedrate"));
	_shootCD = value->intValue();

	// 战斗机的血
	value = dict->valueForKey(Util::format(id + 1, "plane", "_hp"));
	_hp = value->intValue();

	// 战斗机的图片纹理
	value = dict->valueForKey(Util::format(id + 1, "plane"));
	Sprite::initWithFile(value->getCString());

	// 专门发射子弹
	scheduleUpdate();

	_curCD = 0;

	return true;
}

bool Hero::canFire()
{
	// 子弹发射控制
	_curCD++;
	if (_curCD != _shootCD)
		return false;
	_curCD = 0;
	return true;
}

Sprite* Hero::genBullet()
{
	Sprite* bullet = Sprite::create(IMAGE_Bullet);
	getParent()->addChild(bullet); // SceneGame层
	_bullets->addObject(bullet); 
	return bullet;
}

void Hero::fireUp(const Point& delta)
{
	// 发射子弹
	Sprite* bullet = genBullet();
	bullet->setPosition(getPosition() + Vec2(0, getContentSize().height / 2) + delta);
	bullet->runAction(MoveBy::create(4, Vec2(0, winSize.height)));
}

void Hero::fireUp(int angle, const Point& delta)
{
	float x = sinf(angle*M_PI / 180)*winSize.height;
	float y = cosf(angle*M_PI / 180)*winSize.height;

	// 子弹初始位置
	Sprite* bullet = genBullet();
	bullet->setPosition(getPosition() + Vec2(0, getContentSize().height / 2)+delta);

	bullet->runAction(MoveBy::create(4, Vec2(x, y)));
	bullet->setRotation(angle);
}

void Hero::fire0()
{
	fireUp();
}


void Hero::fire1()
{
	fireUp();
	fireUp(Vec2(getContentSize().width / 4, -getContentSize().height/3));
	fireUp(Vec2(-getContentSize().width / 4, -getContentSize().height / 3));
}
void Hero::fire2()
{
	fireUp();
	fireUp(30);
	fireUp(-30);
}

void Hero::fire3()
{
	Point ptDelta = Vec2(0, -getContentSize().height / 2);

	fireUp();
	fireUp(90, ptDelta+Vec2(getContentSize().width/2, 0));
	fireUp(180, ptDelta+ptDelta);
	fireUp(270, ptDelta+Vec2(-getContentSize().width/2, 0));
}

void Hero::fire4()
{
	Point ptDelta = Vec2(0, -getContentSize().height / 2);
	float off = getContentSize().height / 4;
	fireUp(45, ptDelta+Vec2(off, off));
	fireUp(135, ptDelta+Vec2(off, -off));
	fireUp(225, ptDelta+Vec2(-off, -off));
	fireUp(315, ptDelta+Vec2(-off, off));
}

void Hero::fire()
{
	// 发射子弹，要根据不同飞机来发射不同的子弹
	switch (_id)
	{
	case 0:
		fire0();
		break;
	case 1:
		fire1();
		break;
	case 2:
		fire2();
		break;
	case 3:
		fire3();
		break;
	case 4:
		fire4();
		break;
	}
}

void Hero::update(float)
{
	if(canFire())
		fire();

	checkBullets();
}

void Hero::checkBullets()
{
	// 检查子弹是否飞出窗外，如果飞出窗外那么要销毁
	int count = _bullets->count();

	// 当一个__Array要在遍历中，删除一些对象时，应该逆序遍历
	for (int i = count - 1; i >= 0; --i)
	{
		Sprite* bullet = (Sprite*)_bullets->getObjectAtIndex(i);

		// 判断子弹是否整体出了窗口
		if (Util::isAllOutOfWindow(bullet))
		{
			// 删除子弹
			_bullets->removeObjectAtIndex(i);
			bullet->removeFromParent();
		}
	}
}
