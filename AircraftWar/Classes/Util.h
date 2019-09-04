
#ifndef __Util_H__
#define __Util_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define ONE_OVER_TWO (1 / 2.0f)
#define originPos Director::getInstance()->getVisibleOrigin()
#define winSize Director::getInstance()->getVisibleSize()
#define switchScene(__layer_name__, ...)  \
	Director::getInstance()->replaceScene(Util::scene(__layer_name__::create(__VA_ARGS__)));

class Util
{
public:
	static Scene* scene(Layer* layer)
	{
		Scene* s = Scene::create();
		s->addChild(layer);
		return s;
	}

	static Sprite* addBackground(const char* picFile, Node* parent)
	{
		Sprite* sp = Sprite::create(picFile);
		parent->addChild(sp);
		sp->setPosition(Vec2(winSize.width * ONE_OVER_TWO, winSize.height * ONE_OVER_TWO));
		return sp;
	}

	static void modifyLabel(LabelAtlas* label, int change)
	{
		const char* sValue = label->getString().c_str();
		int value = atoi(sValue);
		value += change;
		label->setString(format(value));
	}

	static void moveMenuItemRB(MenuItem* node)
	{
		node->setPosition(
			Vec2(winSize.width / 2 - node->getContentSize().width / 2,
			node->getContentSize().height / 2 - winSize.height / 2)
			);
	}

	static MenuItem* addMenuItemImageRB(const char* picFile1, const char* picFile2, Node* parent)
	{
		MenuItem* item = MenuItemImage::create(picFile1, picFile2);
		Menu* menu = Menu::createWithItem(item);
		parent->addChild(menu);
		moveMenuItemRB(item);
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
		return t->getLocation().getDistanceSq(t->getStartLocation()) < 25;
	}

	static void moveNode(Node* node, const Point& delta)
	{
		node->setPosition(node->getPosition() + delta);
	}

	// 判断node经过移动delta的距离后，是不是部分跑到窗口外面去了
	static bool isPartOutOfWindow(Node* node, const Point& delta=Vec2(0, 0))
	{
		Rect rcNode = node->getBoundingBox();
		Point pt[4];
		pt[0] = Vec2(rcNode.getMinX(), rcNode.getMinY());
		pt[1] = Vec2(rcNode.getMinX(), rcNode.getMaxY());
		pt[2] = Vec2(rcNode.getMaxX(), rcNode.getMaxY());
		pt[3] = Vec2(rcNode.getMaxX(), rcNode.getMinY());

		Rect rcWin(0, 0, winSize.width, winSize.height);
		for (int i = 0; i < 4; i++)
		{
			if (!rcWin.containsPoint(pt[i] + delta))
				return true;
		}

		return false;
	}

	// 判断节点node,通过delta的位移之后，是不是整体移出了窗口
	// 矩形和矩形碰撞
	static bool isAllOutOfWindow(Node* node, const Point& delta = Vec2(0, 0))
	{
		Rect rcWin(0, 0, winSize.width, winSize.height);
		Rect rcNode = node->getBoundingBox();
		rcNode.origin = rcNode.origin + delta;

		if (rcWin.intersectsRect(node->getBoundingBox()))
		{
			return false;
		}
		return true;
	}

};


#endif