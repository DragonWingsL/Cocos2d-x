
#include "SceneMenu.h"
#include "SceneStart.h"
#include "ScrollBackground.h"
#include "SceneGame.h"

bool SceneMenu::init()
{
	Layer::init();

	setPosition(originPos);

	// 增加滚动背景
	ScrollBackground* bg = ScrollBackground::create();
	addChild(bg);

	// 增加一个滚动层，选择飞机
	initScrollMenu();

	// 右下角的返回按钮
	MenuItem* item = Util::addMenuItemImageRB(IMAGE_btn_back_normal, IMAGE_btn_back_press, this);
	item->setTarget(this, menu_selector(SceneMenu::Back));

	return true;
}

void SceneMenu::initScrollMenu()
{
	Node* node = Node::create();
	_node = node;
	// 增加滚动内容
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("chooselevel.plist");
	
	for (int i = 0; i < 5; ++i)
	{
		Sprite* sprite = Sprite::createWithSpriteFrameName(
			Util::format(i + 1, "ChooseLevel", ".png")
			);
		node->addChild(sprite);
		sprite->setPosition(Vec2(winSize.width * ONE_OVER_TWO + i * winSize.width, 
							winSize.height * ONE_OVER_TWO));
		sprite->setTag(i);
	}
	
	// 设置滚动视图
	ScrollView* view = ScrollView::create(winSize, node);
	addChild(view,0);
	view->setDirection(ScrollView::Direction::HORIZONTAL);
	view->setContentSize(CCSize(winSize.width * 5, winSize.height));
	
	view->setSwallowTouches(false);
	
	auto listenerOfView = EventListenerTouchOneByOne::create();
	
	listenerOfView->onTouchBegan = [](Touch *touch, Event *event) -> bool {
		if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation())) {
			return true;
		}
		return false;
	};
	listenerOfView->onTouchEnded = [this](Touch *touch, Event *event) {
		onTouchEnded(touch, event);
	};

	Director::getInstance()
		->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(listenerOfView, view);
		
	// 去掉弹性
	view->setBounceable(false);
}

void SceneMenu::adjustScrollView(float)
{
	int curPos = _node->getPositionX();
	int minOff = 10 * winSize.width;
	int i;
	for (i = 0; i < 5; ++i)
	{
		int adjustPos = -i*winSize.width;
		int offset = abs(curPos - adjustPos);

		// 通过算法查找最接近的调整位置
		if (offset < minOff) minOff = offset;
		else break;
	}

	int adjustPos = -(i - 1)*winSize.width;

	Action* action = MoveTo::create(.1f, Vec2(adjustPos, _node->getPositionY()));
	_node->stopAllActions();
	_node->runAction(action);
}

void SceneMenu::onTouchEnded(Touch* t, Event*)
{
	if (Util::isClick(t))
	{
		// 点击
		Point ptInNode = _node->convertTouchToNodeSpace(t);
		for (int i = 0; i < 5; ++i)
		{
			Node* sprite = _node->getChildByTag(i);
			if (sprite->getBoundingBox().containsPoint(ptInNode))
			{
				switchScene(SceneGame, i);
				break;
			}
		}
	}
	else
	{
		// 滑动：启动一个定时器，让定时器处理函数去矫正ScrollView的位置
		scheduleOnce(schedule_selector(SceneMenu::adjustScrollView), .1f);
	}
	
}

void SceneMenu::Back(Ref*)
{
	switchScene(SceneStart);
}
