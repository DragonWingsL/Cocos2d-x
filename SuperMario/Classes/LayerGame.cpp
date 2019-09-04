
#include "LayerGame.h"
#include "Item.h"
#include "LayerStart.h"
#include "ItemBoss.h"
#include "ItemBullet.h"
#include "GameMenu.h"

void LayerGame::onEnter()
{
	Layer::onEnter();

	setPosition(originPos);

	//初始化地图
	loadMap();
	//添加控制的按钮
	loadControls();
	//加载各种资源
	loadResource();
	//初始化马里奥
	initMario();
	//添加道具
	initItem();

	scheduleUpdate();

	//添加背景音乐
	
	float value = UserDefault::getInstance()->getFloatForKey("MusicValue", .5f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value);
	CCLOG("music:%d", value);
	value = UserDefault::getInstance()->getFloatForKey("EffectValue", .5f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(value);
	CCLOG("effect: %d", value);
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("OnLand.wma", true);
#ifdef DEBUG
//	int offset = 111 * 16;
//Util::moveNode(_Mario, Vec2(offset, 0));
//	Util::moveNode(_map, Vec2(-offset, 0));
#endif

}

void LayerGame::initItem()
{
	_item = __Array::create();
	_item->retain();

	_bridgePoint = Vec2(-1, -1);

	TMXObjectGroup* group = _map->getObjectGroup("objects");
	ValueVector objects = group->getObjects();
	for (const auto &obj : objects)
	{
		ValueMap dict = (ValueMap)obj.asValueMap();
		
		const std::string type = dict["type"].asString();
		//如果是马里奥就不用加载了
		if (type == "BirthPoint")
			continue;
		if(type == "bridgestartpos")
		{
			_bridgePoint.x = dict["x"].asInt() + 16;
			_bridgePoint.y = dict["y"].asInt() - 16;
			continue;
		}

		Item* item = Item::create(dict);
		if(item) 
		{
			_item->addObject(item);
			_map->addChild(item);
		}
			

	}


}

void LayerGame::loadAnimation(const char* picName, const char* frameName, Size picSize, float frameDetal, int frameCount,int frameIdx)
{
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(picName);
	
	Vector<SpriteFrame *> frames;
	for (int i = frameIdx; i < frameIdx + frameCount; i++)
	{
		//一个一个的图片进行扣取
		SpriteFrame* frame = SpriteFrame::createWithTexture(texture, Rect(i * picSize.width, 0, picSize.width, picSize.height));
		frames.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(frames, frameDetal);
	//把动作加载到缓存 用于以后调用
	AnimationCache::getInstance()->addAnimation(animation, frameName);

}


void LayerGame::loadResource()
{
	//load small Mario 
	loadAnimation(IMAGE_Mario_smallWalkRight, ANI_Mario_smallWalkRight, Size(14, 16), .05f, 10);
	loadAnimation(IMAGE_Mario_smallWalkLeft, ANI_Mario_smallWalkLeft, Size(14, 16), .05f, 10);

	//load big Mario
	loadAnimation(IMAGE_Mario_bigWalkRight, ANI_Mario_bigWalkRight, Size(18, 32), .05f, 10);
	loadAnimation(IMAGE_Mario_bigWalkLeft, ANI_Mario_bigWalkLeft, Size(18, 32), .05f, 10);

	//load small Mario fly frame
	loadFrame(IMAGE_Mario_smallWalkLeft, FRAME_Mario_smallFlyLeft, Size(14, 16), 10);
	loadFrame(IMAGE_Mario_smallWalkRight, FRAME_Mario_smallFlyRight, Size(14, 16), 10);

	//load big Mario fly frame
	loadFrame(IMAGE_Mario_bigWalkLeft, FRAME_Mario_bigFlyLeft, Size(18, 32), 10);
	loadFrame(IMAGE_Mario_bigWalkRight, FRAME_Mario_bigFlyRight, Size(18, 32), 10);

	//load mushroom die frame for two status
	loadFrame(IMAGE_ITEM_mushroom, FRAME_ITEM_mushroom_dead1, Size(16, 16), 2);
	loadFrame(IMAGE_ITEM_mushroom, FRAME_ITEM_mushroom_dead2, Size(16, 16), 3);

	//load mushroom animation
	loadAnimation(IMAGE_ITEM_mushroom, ANI_ITEM_mushroom_run, Size(16, 16), .05f, 2);

	//load tortoise animation left right dead
	loadAnimation(IMAGE_ITEM_tortoise, ANI_ITEM_tortoise_left, Size(18, 24), .05f, 4);
	loadAnimation(IMAGE_ITEM_tortoise, ANI_ITEM_tortoise_right, Size(18, 24), .05f, 4, 4);
	loadAnimation(IMAGE_ITEM_tortoise, ANI_ITEM_tortoise_dead, Size(18, 24), .05f, 2, 8);

	//load flower animation
	loadAnimation(IMAGE_ITEM_flower, ANI_ITEM_flower, Size(16, 24), .3f, 2);

	//load mushroomReward
	loadFrame(IMAGE_ITEM_mushroomReward, FRAME_ITEM_mushroomR_big, Size(16, 16), 0);
	loadFrame(IMAGE_ITEM_mushroomReward, FRAME_ITEM_mushroomR_add, Size(16, 16), 1);
	loadFrame(IMAGE_ITEM_mushroomReward, FRAME_ITEM_mushroomR_god, Size(16, 16), 2);

	//load flyfish left and right
	loadAnimation(IMAGE_ITEM_flyfish_Left, ANI_ITEM_fish_Left, Size(16, 16), .05f, 6);
	loadAnimation(IMAGE_ITEM_flyfish_Right, ANI_ITEM_fish_Right, Size(16, 16), .05f, 6);

	//load mario small die
	loadAnimation(IMAGE_Mario_smallDie, ANI_Mario_smallDie, Size(16, 18), .15f, 7);

	//load boss left and right
	loadAnimation(IMAGE_ITEM_boss, ANI_ITEM_boss_left,	Size(32, 32), .05f, 4);
	loadAnimation(IMAGE_ITEM_boss, ANI_ITEM_boss_right, Size(32, 32), .05f, 4, 4);

	//load boss bullet right and left
	loadAnimation(IMAGE_ITEM_boss_bullet, ANI_ITEM_boss_bullet_left,	 Size(24, 8), .05f, 2);
	loadAnimation(IMAGE_ITEM_boss_bullet, ANI_ITEM_boss_bullet_right, Size(24, 8), .05f, 2, 2);

#if 0


	//加载资源
	//加载马里奥的帧缓存
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(IMAGE_Mario_smallWalkRight);
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();//精灵帧缓存

	for (int i = 0; i < 11; i++)
	{
		//一个一个的图片进行扣取
		SpriteFrame* frame = SpriteFrame::createWithTexture(texture, Rect(i*14, 0, 14, 16));
		cache->addSpriteFrame(frame, Util::format(i+1, FRAME_MarioName));
	}

	//加载向右边移动的动作
	__Array* arrFrame = __Array::create();
	for (int i = 0; i < 10; i++)
	{
		//从内存进行获取帧
		SpriteFrame* frame = cache->getSpriteFrameByName(Util::format(i+1, FRAME_MarioName));
		arrFrame->addObject(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(arrFrame, .05f);
	//把动作加载到缓存 用于以后调用
	AnimationCache::getInstance()->addAnimation(animation, ANI_Mario_smallWalkRight);

#endif

}

void LayerGame::initMario()
{
	_Mario = Mario::create();
	_map->addChild(_Mario);
	_Mario->setLocalZOrder(1000);

	_Mario->setBig(_bBig);
	_Mario->updateMario();

	//读取马里奥的信息
	//通过地图资源tmx进行获取马里奥的信息
	TMXObjectGroup* group = _map->getObjectGroup("objects");
	//获取里面的内容
	ValueVector objects = group->getObjects();
	for(const auto &obj : objects)
	{
		ValueMap dict = (ValueMap)obj.asValueMap();
		//如果读取的type 是马里奥的资源 就 进行获取内容
		if (dict["type"].asString() == "BirthPoint")
		{
			const int x = dict["x"].asInt();
			const int y = dict["y"].asInt();
			getMario()->setAnchorPoint(Vec2(0, 0));
			getMario()->setPosition(Vec2(x, y-16));
		}
	}


}

void LayerGame::loadFrame(const char* picName, const char* frameName, Size frameSize, int idx /* = 0 */)
{
	Texture2D* teture = Director::getInstance()->getTextureCache()->addImage(picName);
	SpriteFrame* frame = SpriteFrame::createWithTexture(teture, Rect(idx*frameSize.width, 0, frameSize.width, frameSize.height));
	SpriteFrameCache::getInstance()->addSpriteFrame(frame, frameName);
}

void LayerGame::loadMap()
{
	//加载游戏地图
	_map = TMXTiledMap::create(Util::format(_index+1, "MarioMap", ".tmx"));
	addChild(_map);
	
	//地图向上移动
	float fy = (winSize.height - winSize.height * .4f - _map->getContentSize().height) / _map->getContentSize().height;
	_map->setScale(1.0f + fy);
	_map->setPositionY(_map->getPositionY() + winSize.height * .4f);
}

void LayerGame::loadControls()
{
	//添加一个游戏控制的面板
	Node* node = Util::addBackground(IMAGE_CONTROLS_UI, this);

	node->setAnchorPoint(Vec2(0, 0));
	node->setPosition(Vec2(0, 0));
	node->setContentSize(Size(winSize.width, winSize.height * .4f));
	
	//添加方向键 //不同设置ZOrder 因为这个是最后一个加载进去的所以在最上面
	_ctrl = LayerDirCtrl::create();
	addChild(_ctrl);
	_ctrl->setDelegate(this);

	//添加暂停菜单
	MenuItem* gameMenu = Util::createButtonBk("M_n.png", "M_s.png", this);
	gameMenu->setTarget(this, menu_selector(LayerGame::gameMenu));
	Util::moveNode(gameMenu, Vec2(-winSize.width/2 + 10, 30-10));	
}

void LayerGame::gameMenu(Ref*)
{
	Director::getInstance()->pause();
	GameMenu* menu = GameMenu::create();
	addChild(menu);
}

void LayerGame::onExit()
{
	Layer::onExit();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	_item->release();
}

void LayerGame::runLeft()
{
	_dir = Common::DIR_LEFT;
}

void LayerGame::runRight()
{
	_dir = Common::DIR_RIGHT;
}

void LayerGame::stop()
{
	_dir = Common::DIR_NONE;
}

void LayerGame::checkFlag()
{
	//获得马里奥的右边点
	//与旗子的点进行碰撞检测
	Rect rc = _Mario->getBoundingBox();
	Vec2 ptInRight = Vec2(rc.getMaxX() - 8, rc.getMinY());
	int gid = Util::getmapGID(_map, "flagpole", ptInRight);
		//碰到旗子
	//停止马里奥的全部动作 停止对马里奥的控制 
	//旗子开始滑动下来
	_Mario->setTouchFlag(gid != 0);


}

void LayerGame::checkMarioDead()
{
	Rect rc = _Mario->getBoundingBox();
	Vec2 ptMario = Vec2(rc.getMaxX(), rc.getMaxY());
	if(ptMario.y < 0) //超出界外
	{
		switchScene(LayerStart);
	}
}

void LayerGame::destoryBridge()
{
	//没有桥点
	if(_bridgePoint.x < 0)
		return;

	int gid = Util::getmapGID(_map, "land", _bridgePoint);
	for(int i = 0; ; i++)
	{
		int gid2 = Util::getmapGID(_map, "land", _bridgePoint + Vec2(i*16, 0));
		//判断后面是不是桥 不是桥就直接返回
		if(gid == gid2)
		{
			TMXLayer* layer = _map->getLayer("land");
			layer->setTileGID(0, Util::GL2map(_map, _bridgePoint + Vec2(i*16, 0))); //把桥变成空的
		}
		else
			break;
	}
}

void LayerGame::checkCoin()
{
	Vec2 pt[4];
	Rect rc = _Mario->getBoundingBox();

	//获取马里奥的四个角点 进行检测是否碰到了金币
	pt[0] = Vec2(rc.getMinX(), rc.getMaxY());
	pt[1] = Vec2(rc.getMinX(), rc.getMinY());
	pt[2] = Vec2(rc.getMaxX(), rc.getMaxY());
	pt[3] = Vec2(rc.getMaxX(), rc.getMinY());

	//获取硬币层
	TMXLayer* layerCoin = _map->getLayer("coin");
	for(int i = 0; i < 4; ++i)
	{
		//获取这个点在地图的内容 如果是金币就是吃到了
		Vec2 ptInMap = Util::GL2map(_map, pt[i]);
		int gid = layerCoin->getTileGIDAt(ptInMap);
		if(gid != 0)
		{
			layerCoin->setTileGID(0, ptInMap);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("EatCoin.wma");
		}
	}

}

void LayerGame::update(float dt)
{
#ifdef WIN32
	if(dt > .1f) return;

	_dir = Common::DIR_NONE;

	short keyDown;
	keyDown = GetKeyState('A');
	if (keyDown < 0) _dir =  Common::DIR_LEFT;
	keyDown = GetKeyState('D');
	if (keyDown < 0) _dir =  Common::DIR_RIGHT;
	keyDown = GetKeyState('J');
	if (keyDown < 0) _Mario->jump();

#endif
	if(_Mario->getAutoRun())
		_dir = Common::DIR_RIGHT;

	//判断方向进行运动Mario
	if (_dir == Common::DIR_LEFT)
	{
		_Mario->runLeft(dt);
	}
	else if(_dir == Common::DIR_RIGHT)
	{
		_Mario->runRight(dt);
	}
	else if(_dir == Common::DIR_NONE)
	{
		_Mario->stop(dt);
	}

	_Mario->runUp(dt);
	_Mario->runDown(dt);
	checkFlag();
	checkMarioDead();
	checkCoin();

	int count = _item->count();
	//遍历每个item 
	for (int i = count - 1; i >= 0; --i)
	{
		Item* item = (Item*)_item->getObjectAtIndex(i);
		
		//判断怪物是否是死了
		if(!(item->_bDead))
		{
			//如果没死才执行下面
			item->move(dt);
			//碰撞检测
			item->collision(_Mario);
		}

		//对终点是特殊处理
		if(_Mario->_ptInFinal.x < 0 && item->_type == Item_finalpoint)
		{
			_Mario->_ptInFinal = item->getPosition();
		}


		//怪物是否出了窗口 //右上角的点
		Rect rc = item->getBoundingBox();
		Vec2 ptInRight = Vec2(rc.getMaxX(), rc.getMaxY());
		Vec2 ptInWorld = _map->convertToWorldSpace(ptInRight);
		if(ptInWorld.x < 0 || ptInWorld.y < 0)
		{
			//出了游戏界面就进行删除
			_item->removeObject(item);
			item->removeFromParent();
		}

		//子弹的特殊处理
		if(item->_type == Item_bullet)
		{
			//子弹的最左边的点已经飞出了地图外 回收子弹
			if(rc.getMinX() > _map->getContentSize().width)
			{
				_item->removeObject(item);
				item->removeFromParent();
			}
		}

	}

}

void LayerGame::Jump()
{
	//实际才跳跃了75个像素 不知道怎算的。。。
	_Mario->jump();
}

void LayerGame::Fire()
{

}

void LayerGame::victory()
{
	if(_index > 8)
		return; //游戏通关啦 没有关卡了
	switchScene(LayerGame, _index + 1, _Mario->getBig());
}

void LayerGame::fireBullet(ItemBoss* boss)
{
	ItemBullet* bullet = ItemBullet::create(boss);
	_item->addObject(bullet);
	_map->addChild(bullet);
}