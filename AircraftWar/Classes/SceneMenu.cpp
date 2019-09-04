
#include "SceneMenu.h"
#include "SceneStart.h"
#include "ScrollBackground.h"
#include "SceneGame.h"

bool SceneMenu::init()
{
	Layer::init();

	setPosition(originPos);

	// ���ӹ�������
	ScrollBackground* bg = ScrollBackground::create();
	addChild(bg);

	// ����һ�������㣬ѡ��ɻ�
	initScrollMenu();

	// ���½ǵķ��ذ�ť
	MenuItem* item = Util::addMenuItemImageRB(IMAGE_btn_back_normal, IMAGE_btn_back_press, this);
	item->setTarget(this, menu_selector(SceneMenu::Back));

	return true;
}

void SceneMenu::initScrollMenu()
{
	Node* node = Node::create();
	_node = node;
	// ���ӹ�������
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
	
	// ���ù�����ͼ
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
		
	// ȥ������
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

		// ͨ���㷨������ӽ��ĵ���λ��
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
		// ���
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
		// ����������һ����ʱ�����ö�ʱ��������ȥ����ScrollView��λ��
		scheduleOnce(schedule_selector(SceneMenu::adjustScrollView), .1f);
	}
	
}

void SceneMenu::Back(Ref*)
{
	switchScene(SceneStart);
}
