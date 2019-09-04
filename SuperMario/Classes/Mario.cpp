
#include "Mario.h"
#include "LayerGame.h"
#include "ItemMushroomReward.h"
#include "Item.h"
#include "LayerStart.h"

bool Mario::init()
{
	//���ڴ��л�ȡ����֡ ���д��������
	Sprite::init();

	/*
	CC_PROPERTY(Common::DIR, _dir, Dir); //�˶�����
	CC_SYNTHESIZE(Common::DIR, _face, Face); //����λ��
	CC_SYNTHESIZE(bool, _big, Big); //���
	CC_SYNTHESIZE(bool, _air, Air); //�Ƿ��ڿ���
	CC_SYNTHESIZE(bool, _shoot, Shoot); //

	enum {DEAD_NONE, DEAD_FALL, DEAD_COLLISTION}; //û���� ��������գ� ײ����
	CC_SYNTHESIZE(int, _dead, Dead); //�Ƿ�����
	CC_SYNTHESIZE(int, _speed, Speed); //ˮƽ�����ٶ�
	CC_SYNTHESIZE(int, _speedUp, SpeedUp); //��Ծ���ٶ�
	CC_SYNTHESIZE(int, _speedDown, SpeedDown); //�½����ٶ�
	CC_SYNTHESIZE(int, _gravity, Gravity); //Mario������ 
	*/

	_dir = Common::DIR_NONE;
	_face = Common::DIR_RIGHT;
	_big = false;
	_air = false;
	_shoot = false;

	_dead = DEAD_NONE;
	_speed = 100; //�����ƶ����ٶ�
	_speedUp = 0; //���в���
	_gravity = 10; //Ĭ������
	_speedDown = _gravity; //�ڿ��в���
	_touchFlag = false;
	_bAutoCtrl = false;
	_bAutoRun = false;
	_bProtected = false;
	_onLadder = false;
	_ptInFinal = Vec2(-1, -1);
	_bJump = false;
	_speedV = 60;

	updateMario();

	return true;
}

void Mario::runRight(float dt)
{
	if(_dead != DEAD_NONE)
		return;

	//���������������
	if(_bAutoCtrl)
		return;

	setDir(Common::DIR_RIGHT);

	if (!canMoveRight(dt))
		return;

	setPositionX(getPositionX() + dt * _speed);
	//������ͼ
	Vec2 ptInMap = getPosition();
	Vec2 ptInWorld = getMap()->convertToWorldSpace(ptInMap);

	if (ptInWorld.x > winSize.width / 2)
	{
		getMap()->setPositionX(getMap()->getPositionX() - dt * ((1 + getMap()->getScale()) * _speed));
	}
	
#if 0
	if (_dir != Common::DIR_RIGHT)
	{
		_dir = Common::DIR_RIGHT;
		//�����ȼ����ж���
		Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_smallWalkRight)); //ͨ�����ֽ������ڴ��ö���
		runAction(RepeatForever::create(animation));
	}
#endif

}

void Mario::runLeft(float dt)
{
	if(_dead != DEAD_NONE)
		return;

	//���������������
	if(_bAutoCtrl)
		return;

	setDir(Common::DIR_LEFT);
	//�ж��ƶ����Ƿ����ϰ���
	if (!canMoveLeft(dt))
		return;

	setPositionX(getPositionX()- dt * _speed);
	//TODO
}

bool Mario::getTouchFlag() const
{
	return _touchFlag;
}

void Mario::setTouchFlag(bool var)
{
	//���жϾͻ�һֱ������ ��Ϊ���ظ���ֵ��	_speedUp = _speedDown = 0; 
	if(_touchFlag == var)
		return;

	_touchFlag = var;
	if(_touchFlag)
	{
		_bAutoCtrl = true;
		_speedUp = _speedDown = 0; //��������Ͼ��½���
	}
}

void Mario::update(float dt)
{
	if(_bJump)
	{
		if(getPositionY() < _ptInFinal.y-16)
		{
			if(!getAir())
			{
				setAir(true);
				updateMario();
			}
			setPositionY(getPositionY() + dt*_speedV);
		}
		else if( getPositionY() >  (_ptInFinal.y-16))
		{
			setAir(false);
			setDir(Common::DIR_RIGHT);
			setLocalZOrder(0);
			updateMario();
		}
	}

	setPositionX(getPositionX() + dt * getSpeed());
}

bool Mario::canMoveLeft(float dt)
{
	//�ж������� �ƶ����Ƿ������ϰ���
	Rect rcMario = this->getBoundingBox();
	//��ȡ�ƶ����λ��
	rcMario.origin.x -= dt*getSpeed();
	//����ұ�������
	Vec2 pt[3];
	pt[0] = Vec2(rcMario.getMinX(), rcMario.getMidY());
	pt[1] = Vec2(rcMario.getMinX(), rcMario.getMinY());
	pt[2] = Vec2(rcMario.getMinX(), rcMario.getMaxY());

	TMXTiledMap* map = getMap();
	Vec2 ptInWorld = map->convertToWorldSpace(pt[0]);
	//�ж�������Ƿ���˵�ͼ
	if (ptInWorld.x <= 0) return false;
	
	//����ƶ����ͼ������
	char* layer[] = {"block", "pipe", "land"};
	for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int gid = Util::getmapGID(map, layer[i], pt[j]);
			//������������� ������ƶ����λ�ò��ǿյľ������ϰ��� 0�����ǿ�
			if (gid != 0)
				return false;
		}
	}
	return true;
}

bool Mario::canMoveRight(float dt)
{
	//�ж������� �ƶ����Ƿ������ϰ���
	Rect rcMario = this->getBoundingBox();
	//��ȡ�ƶ����λ��
	rcMario.origin.x += dt * _speed;
	//����ұ�������
	Vec2 pt[3];
	pt[0] = Vec2(rcMario.getMidX(), rcMario.getMidY());
	pt[1] = Vec2(rcMario.getMaxX(), rcMario.getMinY());
	pt[2] = Vec2(rcMario.getMaxX(), rcMario.getMaxY());

	//����ƶ����ͼ������
	TMXTiledMap* map = getMap();
	char* layer[] = {"block", "pipe", "land"};
	for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int gid = Util::getmapGID(map, layer[i], pt[j]);
			//������������� ������ƶ����λ�ò��ǿյľ������ϰ��� 0�����ǿ�
			if (gid != 0)
				return false;
		}
	}
	return true;
}

void Mario::setDir(Common::DIR dir )
{
	if (_dir == dir) return;
		_dir = dir;

	if (_dir != Common::DIR_NONE)
		_face = dir;

	updateMario();
}

Common::DIR Mario::getDir() const
{
	return _dir;
}

void Mario::stop(float dt)
{
	//ֹͣ����
	setDir(Common::DIR_NONE);
}

void Mario::updateMarioFire()
{
#if 0
	if (_big)
	{
		if (_air)
		{
			if (_dir == Common::DIR_RIGHT)
			{
			}
			else if (_dir == Common::DIR_LEFT)
			{
			}
			else if (_dir == Common::DIR_NONE)
			{
			}
		}
		else //�ڵ���
		{
			if (_dir == Common::DIR_RIGHT)
			{
			}
			else if (_dir == Common::DIR_LEFT)
			{
			}
			else if (_dir == Common::DIR_NONE)
			{
			}
		}
	}
	else //С��Mario
	{
		if (_air)
		{
			if (_dir == Common::DIR_RIGHT)
			{
			}
			else if (_dir == Common::DIR_LEFT)
			{
			}
			else if (_dir == Common::DIR_NONE)
			{
			}
		}
		else //�ڵ���
		{
			if (_dir == Common::DIR_RIGHT)
			{
			}
			else if (_dir == Common::DIR_LEFT)
			{
			}
			else if (_dir == Common::DIR_NONE)
			{
			}
		}
	}

#endif

}

void Mario::updateMarioNoFire()
{
	if (_big)
	{
		if(_dead == DEAD_FALL)
		{
			return;
		}

		if (_air)
		{
			if (_face == Common::DIR_LEFT)
			{
				setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_Mario_bigFlyLeft));
			}
			else if(_face == Common::DIR_RIGHT)
			{
				setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_Mario_bigFlyRight));
			}
		}
		else //�ڵ���
		{
			if (_dir == Common::DIR_RIGHT)
			{
				Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_bigWalkRight)); //ͨ�����ֽ������ڴ��ö���
				runAction(RepeatForever::create(animation));
			}
			else if (_dir == Common::DIR_LEFT)
			{

				Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_bigWalkLeft)); //ͨ�����ֽ������ڴ��ö���
				runAction(RepeatForever::create(animation));
			}
			else if (_dir == Common::DIR_NONE)
			{
				
				if (_face == Common::DIR_LEFT)
				{
					//�õ�һ֡��ֹͣ�����ľ���
					setDisplayFrameWithAnimationName(ANI_Mario_bigWalkLeft, 0);
				}
				else if(_face == Common::DIR_RIGHT)
				{
					//�õ�һ֡��ֹͣ�����ľ���
					setDisplayFrameWithAnimationName(ANI_Mario_bigWalkRight, 0);
				}
					
			}
		}
	}
	else //С��Mario
	{
		if(_dead == DEAD_COLLISTION)
		{
			Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_smallDie));
			JumpBy* Jump = JumpBy::create(.35f, Vec2(0, 0), 30, 1);
			MoveBy* move = MoveBy::create(5, Vec2(0, -winSize.height));
			runAction(CCSequence::create(CCSpawn::create(animate, Jump, NULL), move, NULL));
		}
		else if(_dead == DEAD_FALL)
		{
			return;
		}

		else if (_air)
		{
			if (_face == Common::DIR_LEFT)
			{
					CCLOG("small air face left");
				setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_Mario_smallFlyLeft));
			}
			else if(_face == Common::DIR_RIGHT)
			{
				setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_Mario_smallFlyRight));
			}
#if 0
			//�������û�з���Ķ��� //ֻ��һ��֡�Ķ���
			if (_dir == Common::DIR_RIGHT)
			{
			}
			else if (_dir == Common::DIR_LEFT)
			{
			}
			else if (_dir == Common::DIR_NONE)
			{
			}
#endif
		}
		else //�ڵ���
		{
			if (_dir == Common::DIR_RIGHT)
			{
				Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_smallWalkRight)); //ͨ�����ֽ������ڴ��ö���
				runAction(RepeatForever::create(animation));
			}
			else if (_dir == Common::DIR_LEFT)
			{
				CCLOG("small air face left  -- ����");
				Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_smallWalkLeft)); //ͨ�����ֽ������ڴ��ö���
				runAction(RepeatForever::create(animation));
			}
			else if (_dir == Common::DIR_NONE)
			{
				/* 
				//����Mario�泯�����ͼƬ
				setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(Util::format(1, FRAME_MarioName)));
				*/
				if (_face == Common::DIR_LEFT)
				{
					CCLOG("small air face left ---face");
					//�õ�һ֡��ֹͣ�����ľ���
					setDisplayFrameWithAnimationName(ANI_Mario_smallWalkLeft, 0);
				}
				else if(_face == Common::DIR_RIGHT)
				{
					//�õ�һ֡��ֹͣ�����ľ���
					setDisplayFrameWithAnimationName(ANI_Mario_smallWalkRight, 0);
				}
					
			}
		}
	}

}

void Mario::runUp(float dt)
{
	if (!canMoveUp(dt))
		return;
	
	setPositionY(getPositionY() + dt * getSpeedUp());
	setSpeedUp(getSpeedUp() - getGravity());
}

bool Mario::canMoveUp(float dt)
{

	//û���ٶȲ������ж�
	if (getSpeedUp() <= 0) return false;
	
	//�ж��Ƿ��ж�������
	Rect rcMario = this->getBoundingBox();
	rcMario.origin.y += dt*getSpeedUp();
	//�ж���������������Ƿ�Ϊ��
	Vec2 pt[3];
	pt[0] = Vec2(rcMario.getMidX(), rcMario.getMaxY());
	pt[1] = Vec2(rcMario.getMinX(), rcMario.getMaxY());
	pt[2] = Vec2(rcMario.getMaxX(), rcMario.getMaxY());

	TMXTiledMap* map = getMap();
	const char* layer[] = {"block", "pipe", "land"};
	for (int i = 0; i < sizeof(layer) / sizeof(*layer);  ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int gid = Util::getmapGID(map, layer[i], pt[j]);
			//������������� ������½����λ�ò��ǿյľ������ϰ��� 0�����ǿ�
			if (gid != 0)
			{
				//�����Ծ �Ͷ�󷴵� //���һ��������Ч��
				setSpeedDown(getSpeedUp());
				setSpeedUp(0); //ֹͣ��Ծ����

				//����
				upBlock(layer[i], pt[j]);

				return false;				
			}
		}
	}

	return true;
}

void Mario::upBlock(const char* layerName, Vec2 pt)
{
#if 0
	//��ʱ
	if (strcmp(layerName, "block") == 0)
	{
		Sprite* sBlock  = Util::getMapTileSprite(getMap(), layerName, pt);
		sBlock->runAction(JumpBy::create(.1f, Vec2(0, 0), 10, 1));
	}
#endif

	//����item����˭������½�������ײ
	//�ı����item��״̬
	LayerGame* game = (LayerGame*)getParent()->getParent();
	__Array* arrItem = game->_item;
	Ref* obj;
	CCARRAY_FOREACH(arrItem, obj)
	{
		Item* item = (Item*)obj;
		if(item->_type == Item_MushroomReward || item->_type == Item_MushroomAddLife)
		{
			//���������Ģ������ ����Ģ�����ͳ���
			ItemMushroomReward* reward = (ItemMushroomReward*)obj;
			if(reward->getBoundingBox().containsPoint(pt))
			{
				reward->_status = ItemMushroomReward::GROW;
				MoveBy* move = MoveBy::create(.1f, Vec2(0, 16));
				CallFunc* callfunc = CallFunc::create([reward]() {reward->setStatuMove(); });
				reward->runAction(CCSequence::create(move, callfunc, NULL));

			}


		}


	}


}

void Mario::runDown(float dt)
{
	
	if (!canMoveDown(dt))
		return;

	setPositionY(getPositionY() - dt * getSpeedDown());
	setSpeedDown(getSpeedDown() + getGravity());
}

bool Mario::canMoveDown(float dt)
{
	//�����������޷��½�
	if (getSpeedUp() > 0)
		return false;

	//����ִ���������� 
	if(_dead !=  DEAD_NONE)
		return false;

	//�����������
	if(_onLadder)
		return false;

	//�յ���¥�� ������ڷ�
	if(_bJump)
		return false;

	Rect rcMario = this->getBoundingBox();
	rcMario.origin.y -= dt*getSpeedDown();
	//�ж���������������Ƿ�Ϊ��
	Vec2 pt[3];
	pt[0] = Vec2(rcMario.getMidX(), rcMario.getMinY());
	pt[1] = Vec2(rcMario.getMinX(), rcMario.getMinY());
	pt[2] = Vec2(rcMario.getMaxX(), rcMario.getMinY());
	 
	TMXTiledMap* map = getMap();
	const char* layer[] = {"block", "pipe", "land"};
	for (int i = 0; i < sizeof(layer) / sizeof(*layer);  ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int gid = Util::getmapGID(map, layer[i], pt[j]);
			//������������� ������½����λ�ò��ǿյľ������ϰ��� 0�����ǿ�
			if (gid != 0)
			{
				if(_air)
				{
					//���ϰ���ʹ��������
					setAir(false);
					updateMario();
					setSpeedDown(getGravity());
				}
				return false;				
			}
		}
	}

	return true;
}

void Mario::updateMario()
{
	this->stopAllActions();

	if (_shoot)
	{
		updateMarioFire();
	}
	else
	{
		updateMarioNoFire();
	}

}

void Mario::jump()
{
	//�Ѿ������ӵ�λ�� ����������
	if(_bAutoCtrl)
		return;

	//����Ѿ�������
	if (_air) return;

	if(_dead) return;

	setAir(true);
	setSpeedUp(300);
	updateMario(); //����״̬
}

void Mario::eatItem(Item_Type type)
{
	if(type == Item_MushroomReward)
	{
		//��ɴ����
		_big = true;
		updateMario();
	}
}

void Mario::killed(bool Dead)
{
#ifdef DEBUG
	return;
#endif
	//����ǵ����ˣ�������͸��
	if(Dead)
	{
		switchScene(LayerStart);
	}

	//�������״̬
	if(_bProtected) 
		return;

	if(_big)
	{
		_big = false;
		updateMario();
		//�޵�һ��ʱ�� ��ֹ�ظ���ײ���
		setOpacity(128); //��͸��
		_bProtected = true;
		scheduleOnce(schedule_selector(Mario::setProtectedFalse), .5f);

	}
	else
	{
		_bAutoCtrl = true;
		_bProtected = true;
		setDead(DEAD_COLLISTION);
		updateMario();
	}

}