#include "Hero.h"

bool Hero::init(int id)
{
	// ����ɻ���ID�����ڷ����ӵ�����ͬ�ķɻ������ӵ��ķ�ʽ��һ��
	_id = id;

	// ��ȡ����ɻ��Ļ�����Ϣ������damage��hp�������ӵ���ʱ����������ͼƬ����
	__Dictionary* dict = __Dictionary::createWithContentsOfFile(CFG_planeinfo);

	// ս������ս����
	const __String* value = dict->valueForKey(Util::format(id + 1, "planedamageamount"));
	_damage = value->intValue();

	// ս�����ķ����ӵ���ʱ����
	value = dict->valueForKey(Util::format(id + 1, "planeshootspeedrate"));
	_shootCD = value->intValue();

	// ս������Ѫ
	value = dict->valueForKey(Util::format(id + 1, "plane", "_hp"));
	_hp = value->intValue();

	// ս������ͼƬ����
	value = dict->valueForKey(Util::format(id + 1, "plane"));
	Sprite::initWithFile(value->getCString());

	// ר�ŷ����ӵ�
	scheduleUpdate();

	_curCD = 0;

	return true;
}

bool Hero::canFire()
{
	// �ӵ��������
	_curCD++;
	if (_curCD != _shootCD)
		return false;
	_curCD = 0;
	return true;
}

Sprite* Hero::genBullet()
{
	Sprite* bullet = Sprite::create(IMAGE_Bullet);
	getParent()->addChild(bullet); // SceneGame��
	_bullets->addObject(bullet); 
	return bullet;
}

void Hero::fireUp(const Point& delta)
{
	// �����ӵ�
	Sprite* bullet = genBullet();
	bullet->setPosition(getPosition() + Vec2(0, getContentSize().height / 2) + delta);
	bullet->runAction(MoveBy::create(4, Vec2(0, winSize.height)));
}

void Hero::fireUp(int angle, const Point& delta)
{
	float x = sinf(angle*M_PI / 180)*winSize.height;
	float y = cosf(angle*M_PI / 180)*winSize.height;

	// �ӵ���ʼλ��
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
	// �����ӵ���Ҫ���ݲ�ͬ�ɻ������䲻ͬ���ӵ�
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
	// ����ӵ��Ƿ�ɳ����⣬����ɳ�������ôҪ����
	int count = _bullets->count();

	// ��һ��__ArrayҪ�ڱ����У�ɾ��һЩ����ʱ��Ӧ���������
	for (int i = count - 1; i >= 0; --i)
	{
		Sprite* bullet = (Sprite*)_bullets->getObjectAtIndex(i);

		// �ж��ӵ��Ƿ�������˴���
		if (Util::isAllOutOfWindow(bullet))
		{
			// ɾ���ӵ�
			_bullets->removeObjectAtIndex(i);
			bullet->removeFromParent();
		}
	}
}
