
#ifndef __Ulit_H__
#define __Ulit_H__

#include "Macro.h"
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#include "SimpleAudioEngine.h"

#define originPos Director::getInstance()->getVisibleOrigin()
#define winSize Director::getInstance()->getVisibleSize()
#define switchScene(__LayerScene__, ...) Director::getInstance()->replaceScene(Util::scene(__LayerScene__::create(__VA_ARGS__)))

class Util : Layer
{
public:
	
	static Scene* scene(Layer* layer)
	{
		Scene* scene = Scene::create();
		scene->addChild(layer);
		return scene;
	}

	static Sprite* addBackground(const char* picfile, Node* parent)
	{
		Sprite* sprite = Sprite::create(picfile);
		parent->addChild(sprite);
		sprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
		sprite->setContentSize(winSize);
		return sprite;
	}

	static void moveBK(MenuItem* item)
	{
		item->setPosition(Vec2(winSize.width/2-item->getContentSize().width/2,
											item->getContentSize().height/2-winSize.height/2));
	}
	
	static MenuItem* createButtonBk(const char* bkfile1, const char* bkfile2, Node* parent)
	{
		MenuItem* item = MenuItemImage::create(bkfile1, bkfile2);
		Menu* menu = Menu::create(item, NULL);
		parent->addChild(menu);
		moveBK(item);
		return item;
	}

	static char* format(int value, const char* prefix = "", const char* suffix = "")
	{
		static char buf[1024];
		sprintf(buf, "%s%d%s", prefix, value, suffix);
		return buf;
	}

	static bool isClick(Touch* t)
	{
		return t->getLocation().getDistanceSq(t->getStartLocation()) <= 25;
	}

	static void moveNode(Node* node, Vec2 pt)
	{
		node->setPosition(node->getPosition() + pt);
	}

	static bool isPartOutWindow(Node* node, Touch* t)
	{
		//获得主角当前的矩形位置
		Rect nodeR = node->getBoundingBox();

		//获取四个边点的位置
		Vec2 pt[4];
		pt[0] = Vec2(nodeR.getMinX(), nodeR.getMinY());
		pt[1] = Vec2(nodeR.getMinX(), nodeR.getMaxY());
		pt[2] = Vec2(nodeR.getMaxX(), nodeR.getMinY());
		pt[3] = Vec2(nodeR.getMaxX(), nodeR.getMaxY());

		Rect Rcwin(0, 0, winSize.width, winSize.height);

		for (int i = 0; i < 4; i++)
		{
			if (!Rcwin.containsPoint(pt[i]+t->getDelta()))
			{
				return true;
			}
		}
		return false;
	}

	static bool isOutWindow(Node* node, Vec2 delta = Vec2(0, 0))
	{
		Rect winR(0, 0, winSize.width, winSize.height);
		Rect winN = node->getBoundingBox();
		winN.origin = winN.origin + delta;

		if (winR.intersectsRect(winN))
		{
			return false;
		}
		return true;
	}

	static void ModifLabel(LabelAtlas* label, int change)
	{
		const char* sValue = label->getString().c_str();
		int value = atoi(sValue);
		value += change;
		label->setString(format(value));
	}

	static Vec2 map2GL(TMXTiledMap* map, Vec2 pt)
	{
		//转换成格子坐标
		CCLOG("map->getTileSize().width:%d ", map->getTileSize().width);
		CCLOG("pt.x:%d", pt.x);
		int UIx = pt.x * map->getTileSize().width;
		int UIy = pt.y * map->getTileSize().height;

		int GLx = UIx;
		int GLy = map->getContentSize().height - UIy;
	
		return Vec2(GLx, GLy);
	}

	//获得当前坐标的有什么东西
	static Vec2 GL2map(TMXTiledMap* map, Vec2 pt)
	{
		int x = pt.x / map->getTileSize().width;
		int y = pt.y / map->getTileSize().height;

		y = map->getMapSize().height - 1 - y;

		if (x < 0) x = 0;
		if (x >= map->getMapSize().width) x = 0;
		if (y < 0) y = 0;
		if (y >= map->getMapSize().height) y = 0;

		return Vec2(x, y);
	}

	//判断点是否跑出地图
	static bool mapContainsPoint(TMXTiledMap* map, Vec2 pt)
	{
		Rect rcM(0, 0, map->getContentSize().width, map->getContentSize().height);
			return rcM.containsPoint(pt);
	}

	//通过格子坐标获取格子的物品
	static int getmapGID(TMXTiledMap* map, const char* layername, Vec2 pt)
	{
		TMXLayer* layer = map->getLayer(layername);
		Vec2 ptInMap = GL2map(map, pt);

		//这里用转换的位置 ！！！
		if (ptInMap.x < 0) return 0;
		if (ptInMap.x >= map->getMapSize().width) return 0;
		if (ptInMap.y < 0) return 0;
		if (ptInMap.y >= map->getMapSize().height) return 0;
	
		return  layer->getTileGIDAt(ptInMap);
	}

	static Sprite* getMapTileSprite(TMXTiledMap* map, const char* layerName, Vec2 pt)
	{
		TMXLayer* layer = map->getLayer(layerName);
		Vec2 ptInMap = GL2map(map, pt);

		//这里用转换的位置 ！！！
		if (ptInMap.x < 0) return 0;
		if (ptInMap.x >= map->getMapSize().width) return 0;
		if (ptInMap.y < 0) return 0;
		if (ptInMap.y >= map->getMapSize().height) return 0;

		return  layer->getTileAt(ptInMap);;
	}

};

//重写菜单 做自己的按键控制
class Panel : public Menu
{
public:
	CREATE_FUNC(Panel);

	void update(float )
	{
		this->_selectedItem->activate(); //执行回调函数
	}

	//拦截按下的信息处理
	bool onTouchBegan(Touch* t, Event* ev)
	{
		bool tou = Menu::onTouchBegan(t, ev);
			if (!tou)
			return false;
		
		scheduleUpdate();
		return true;
	}
	void onTouchMoved(Touch* t, Event* ev)
	{
		Menu::onTouchMoved(t, ev);
	}

	void onTouchCancelled(Touch *t, Event* ev)
	{
		unscheduleUpdate();
		Menu::onTouchCancelled(t, ev);
	}
	void onTouchEnded(Touch* t, Event* ev)
	{
		unscheduleUpdate();
		Menu::onTouchEnded(t, ev);
	}

};


#endif