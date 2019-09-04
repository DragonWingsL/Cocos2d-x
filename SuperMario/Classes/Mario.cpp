
#include "Mario.h"
#include "LayerGame.h"
#include "ItemMushroomReward.h"
#include "Item.h"
#include "LayerStart.h"

bool Mario::init()
{
	//在内存中获取精灵帧 进行创建马里奥
	Sprite::init();

	/*
	CC_PROPERTY(Common::DIR, _dir, Dir); //运动方向
	CC_SYNTHESIZE(Common::DIR, _face, Face); //脸的位置
	CC_SYNTHESIZE(bool, _big, Big); //变大
	CC_SYNTHESIZE(bool, _air, Air); //是否在空中
	CC_SYNTHESIZE(bool, _shoot, Shoot); //

	enum {DEAD_NONE, DEAD_FALL, DEAD_COLLISTION}; //没死， 掉落在虚空， 撞死的
	CC_SYNTHESIZE(int, _dead, Dead); //是否死亡
	CC_SYNTHESIZE(int, _speed, Speed); //水平方向速度
	CC_SYNTHESIZE(int, _speedUp, SpeedUp); //跳跃的速度
	CC_SYNTHESIZE(int, _speedDown, SpeedDown); //下降的速度
	CC_SYNTHESIZE(int, _gravity, Gravity); //Mario的重力 
	*/

	_dir = Common::DIR_NONE;
	_face = Common::DIR_RIGHT;
	_big = false;
	_air = false;
	_shoot = false;

	_dead = DEAD_NONE;
	_speed = 100; //地面移动的速度
	_speedUp = 0; //空中才有
	_gravity = 10; //默认体重
	_speedDown = _gravity; //在空中才有
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

	//马里奥碰到旗子了
	if(_bAutoCtrl)
		return;

	setDir(Common::DIR_RIGHT);

	if (!canMoveRight(dt))
		return;

	setPositionX(getPositionX() + dt * _speed);
	//滚动地图
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
		//尝试先简单运行动作
		Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_smallWalkRight)); //通过名字进行在内存获得动作
		runAction(RepeatForever::create(animation));
	}
#endif

}

void Mario::runLeft(float dt)
{
	if(_dead != DEAD_NONE)
		return;

	//马里奥碰到旗子了
	if(_bAutoCtrl)
		return;

	setDir(Common::DIR_LEFT);
	//判断移动后是否有障碍物
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
	//不判断就会一直在天上 因为会重复赋值个	_speedUp = _speedDown = 0; 
	if(_touchFlag == var)
		return;

	_touchFlag = var;
	if(_touchFlag)
	{
		_bAutoCtrl = true;
		_speedUp = _speedDown = 0; //如果在天上就下降吧
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
	//判断三个点 移动后是否碰到障碍物
	Rect rcMario = this->getBoundingBox();
	//获取移动后的位置
	rcMario.origin.x -= dt*getSpeed();
	//获得右边三个点
	Vec2 pt[3];
	pt[0] = Vec2(rcMario.getMinX(), rcMario.getMidY());
	pt[1] = Vec2(rcMario.getMinX(), rcMario.getMinY());
	pt[2] = Vec2(rcMario.getMinX(), rcMario.getMaxY());

	TMXTiledMap* map = getMap();
	Vec2 ptInWorld = map->convertToWorldSpace(pt[0]);
	//判断马里奥是否出了地图
	if (ptInWorld.x <= 0) return false;
	
	//获得移动后地图的物体
	char* layer[] = {"block", "pipe", "land"};
	for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int gid = Util::getmapGID(map, layer[i], pt[j]);
			//如果这三个场景 马里奥移动后的位置不是空的就是有障碍物 0代表是空
			if (gid != 0)
				return false;
		}
	}
	return true;
}

bool Mario::canMoveRight(float dt)
{
	//判断三个点 移动后是否碰到障碍物
	Rect rcMario = this->getBoundingBox();
	//获取移动后的位置
	rcMario.origin.x += dt * _speed;
	//获得右边三个点
	Vec2 pt[3];
	pt[0] = Vec2(rcMario.getMidX(), rcMario.getMidY());
	pt[1] = Vec2(rcMario.getMaxX(), rcMario.getMinY());
	pt[2] = Vec2(rcMario.getMaxX(), rcMario.getMaxY());

	//获得移动后地图的物体
	TMXTiledMap* map = getMap();
	char* layer[] = {"block", "pipe", "land"};
	for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int gid = Util::getmapGID(map, layer[i], pt[j]);
			//如果这三个场景 马里奥移动后的位置不是空的就是有障碍物 0代表是空
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
	//停止动作
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
		else //在地上
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
	else //小的Mario
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
		else //在地上
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
		else //在地上
		{
			if (_dir == Common::DIR_RIGHT)
			{
				Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_bigWalkRight)); //通过名字进行在内存获得动作
				runAction(RepeatForever::create(animation));
			}
			else if (_dir == Common::DIR_LEFT)
			{

				Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_bigWalkLeft)); //通过名字进行在内存获得动作
				runAction(RepeatForever::create(animation));
			}
			else if (_dir == Common::DIR_NONE)
			{
				
				if (_face == Common::DIR_LEFT)
				{
					//用第一帧做停止动作的精灵
					setDisplayFrameWithAnimationName(ANI_Mario_bigWalkLeft, 0);
				}
				else if(_face == Common::DIR_RIGHT)
				{
					//用第一帧做停止动作的精灵
					setDisplayFrameWithAnimationName(ANI_Mario_bigWalkRight, 0);
				}
					
			}
		}
	}
	else //小的Mario
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
			//在天空中没有方向的动作 //只有一个帧的动画
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
		else //在地上
		{
			if (_dir == Common::DIR_RIGHT)
			{
				Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_smallWalkRight)); //通过名字进行在内存获得动作
				runAction(RepeatForever::create(animation));
			}
			else if (_dir == Common::DIR_LEFT)
			{
				CCLOG("small air face left  -- 地上");
				Animate* animation = Animate::create(AnimationCache::getInstance()->getAnimation(ANI_Mario_smallWalkLeft)); //通过名字进行在内存获得动作
				runAction(RepeatForever::create(animation));
			}
			else if (_dir == Common::DIR_NONE)
			{
				/* 
				//设置Mario面朝方向的图片
				setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(Util::format(1, FRAME_MarioName)));
				*/
				if (_face == Common::DIR_LEFT)
				{
					CCLOG("small air face left ---face");
					//用第一帧做停止动作的精灵
					setDisplayFrameWithAnimationName(ANI_Mario_smallWalkLeft, 0);
				}
				else if(_face == Common::DIR_RIGHT)
				{
					//用第一帧做停止动作的精灵
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

	//没有速度不进行判断
	if (getSpeedUp() <= 0) return false;
	
	//判断是否有顶到东西
	Rect rcMario = this->getBoundingBox();
	rcMario.origin.y += dt*getSpeedUp();
	//判断马里奥下面三点是否为空
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
			//如果这三个场景 马里奥下降后的位置不是空的就是有障碍物 0代表是空
			if (gid != 0)
			{
				//多大跳跃 就多大反弹 //造成一个反弹的效果
				setSpeedDown(getSpeedUp());
				setSpeedUp(0); //停止跳跃动作

				//动作
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
	//暂时
	if (strcmp(layerName, "block") == 0)
	{
		Sprite* sBlock  = Util::getMapTileSprite(getMap(), layerName, pt);
		sBlock->runAction(JumpBy::create(.1f, Vec2(0, 0), 10, 1));
	}
#endif

	//遍历item看是谁与马里奥进行了碰撞
	//改变这个item的状态
	LayerGame* game = (LayerGame*)getParent()->getParent();
	__Array* arrItem = game->_item;
	Ref* obj;
	CCARRAY_FOREACH(arrItem, obj)
	{
		Item* item = (Item*)obj;
		if(item->_type == Item_MushroomReward || item->_type == Item_MushroomAddLife)
		{
			//如果类型是蘑菇奖赏 就让蘑菇奖赏出来
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
	//正在上升，无法下降
	if (getSpeedUp() > 0)
		return false;

	//正在执行死亡动作 
	if(_dead !=  DEAD_NONE)
		return false;

	//马里奥在桥上
	if(_onLadder)
		return false;

	//终点在楼上 马里奥在飞
	if(_bJump)
		return false;

	Rect rcMario = this->getBoundingBox();
	rcMario.origin.y -= dt*getSpeedDown();
	//判断马里奥下面三点是否为空
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
			//如果这三个场景 马里奥下降后的位置不是空的就是有障碍物 0代表是空
			if (gid != 0)
			{
				if(_air)
				{
					//有障碍物就代表落地了
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
	//已经到旗子的位置 不能再跳了
	if(_bAutoCtrl)
		return;

	//如果已经上天了
	if (_air) return;

	if(_dead) return;

	setAir(true);
	setSpeedUp(300);
	updateMario(); //更新状态
}

void Mario::eatItem(Item_Type type)
{
	if(type == Item_MushroomReward)
	{
		//变成大个子
		_big = true;
		updateMario();
	}
}

void Mario::killed(bool Dead)
{
#ifdef DEBUG
	return;
#endif
	//如果是掉坑了，就是死透了
	if(Dead)
	{
		switchScene(LayerStart);
	}

	//如果保护状态
	if(_bProtected) 
		return;

	if(_big)
	{
		_big = false;
		updateMario();
		//无敌一段时间 防止重复碰撞检测
		setOpacity(128); //半透明
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