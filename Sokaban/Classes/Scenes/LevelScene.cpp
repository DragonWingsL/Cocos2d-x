
#include <cocos2d.h>
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

#include "UI/Ext/LevelCrate.h"
#include "UI/Label/SpriteLabel.h"
#include "Audio/AudioUtils.h"
#include "User/UserData.h"
#include "GameScene.h"
#include "CommonMacro.h"
#include "GlobalManager.h"
#include "LevelScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene *LevelScene::createScene()
{
	return LevelScene::create();
}

bool LevelScene::init()
{
	if (!Scene::init())
		return false;

	auto manager = GlobalManager::getInstance();

	Layer *defaultLayer = Layer::create();
	defaultLayer->setPosition(manager->VisibleOriginPos);
	addChild(defaultLayer);

	Sprite *background = Sprite::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelBackground));
	background->setPosition(manager->VisibleCenterPos);
	background->setContentSize(manager->VisibleSize);
	defaultLayer->addChild(background);

	SpriteLabel *leftHalf = SpriteLabel::create("SELECT");
	leftHalf->setScale(.5f);
	defaultLayer->addChild(leftHalf);

	SpriteLabel *rightHalf = SpriteLabel::create("LEVEL");
	rightHalf->setScale(.5f);
	defaultLayer->addChild(rightHalf);

	Vec2 partOffset = Vec2(
		-manager->VisibleSize.width * ONE_OVER_TWO * .45f,
		manager->VisibleSize.height * .35f
	);

	leftHalf->setPosition(manager->VisibleCenterPos + partOffset);
	partOffset.x *= -1;
	rightHalf->setPosition(manager->VisibleCenterPos + partOffset);

	Layer *levelPageLayer = Layer::create();
	levelPageLayer->setPosition(manager->VisibleOriginPos);
	addChild(levelPageLayer, 2);

	auto pageView = initLevelCratePageView();
	levelPageLayer->addChild(pageView, 1);

	Layer *menuLayer = Layer::create();
	menuLayer->setPosition(manager->VisibleOriginPos);
	addChild(menuLayer, 3);

	ui::Button *backBtn = ui::Button::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelBackBtnNormal));
	backBtn->setScale(.65f);
	backBtn->setPosition(Vec2(manager->VisibleSize * .15f));
	backBtn->addTouchEventListener([backBtn](Ref *, ui::Widget::TouchEventType eventType) {
		if (eventType == ui::Widget::TouchEventType::ENDED) {
			AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
			Director::getInstance()->popScene();
		}
	});
	menuLayer->addChild(backBtn);

	Menu *pageMenu = Menu::create();
	pageMenu->setPosition(Vec2(pageMenu->getPosition().x, manager->VisibleSize.height * .2f));
	menuLayer->addChild(pageMenu);

	for (int i = 0; i < 3; ++i) {
		MenuItemImage *pageMenuItem = MenuItemImage::create(
			manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelPageBtnNormal),
			manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelPageBtnNormal),
			manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelPageBtnSelected)
		);
		pageMenuItem->setScale(.5f);
		pageMenuItem->setEnabled(true);
		pageMenuItem->setCallback([=, idx = i](Ref *) {
			for (const auto &item : pageMenu->getChildren()) {
				((MenuItem *)item)->setEnabled(true);
			}
			pageView->scrollToPage(idx);
			((MenuItem *)*(pageMenu->getChildren().begin() + idx))->setEnabled(false);
		});
		pageMenu->addChild(pageMenuItem);
	}

	auto firstMenuItem = (MenuItem *)*pageMenu->getChildren().begin();
	firstMenuItem->setEnabled(false);
	pageMenu->alignItemsHorizontallyWithPadding(
		firstMenuItem->getContentSize().width * firstMenuItem->getScaleX()
	);

	pageView->addEventListener([pageView, pageMenu](Ref*, ui::PageView::EventType eventType) {
		if (eventType == ui::PageView::EventType::TURNING) {
			for (const auto &item : pageMenu->getChildren()) {
				((MenuItem *)item)->setEnabled(true);
			}
			((MenuItem *)*(pageMenu->getChildren().begin() + pageView->getCurPageIndex()))->setEnabled(false);
		}
	});

	return true;
}

void LevelScene::updateCrateLock()
{
	auto userData = GlobalManager::getInstance()->getUserData();
	auto children = getChildren();
	for (int i = 0, count = userData->getCurrentLevel(); i < count; ++i) {
		((LevelCrate *)(*(_crates.begin() + i)))->setLock(false);
	}
}

cocos2d::ui::PageView *LevelScene::initLevelCratePageView()
{
	auto manager = GlobalManager::getInstance();

	using namespace ui;
	PageView *pageView = PageView::create();
	pageView->setContentSize(manager->VisibleSize);

	const int veriticalBoxCount = 6, horizontalBoxCount = 5;
	Texture2D *texCrate = Director::getInstance()->getTextureCache()->getTextureForKey(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelLockCrate));
	Size crateSize(texCrate->getContentSize() * .6f);
	Size interval(crateSize * ONE_OVER_TWO);
	Size offset(crateSize + interval);
	offset.height *= -1;

	Size pageSize = Size(
		crateSize.width * 5 + interval.width * 4,
		crateSize.height * 6 + interval.height * 5
	);
	Vec2 pageLTPos = manager->VisibleCenterPos +
		Vec2(-pageSize.width * ONE_OVER_TWO + crateSize.width * ONE_OVER_TWO,
			pageSize.height * ONE_OVER_TWO - crateSize.height * ONE_OVER_TWO);

	auto shareTouchListener = EventListenerTouchOneByOne::create();
	shareTouchListener->onTouchBegan = [pageView](Touch *touch, Event *event) -> bool {
		auto levelBox = dynamic_cast<LevelCrate *>(event->getCurrentTarget());
		do {
			CC_BREAK_IF(
				!levelBox || levelBox->getLock() || 
				(levelBox->getLevelNum() - 1) / 30 != pageView->getCurPageIndex() ||
				!levelBox->getBoundingBox().containsPoint(touch->getLocation())
			);
			return true;
		} while (0);
		return false;
	};
	shareTouchListener->onTouchEnded = [](Touch *touch, Event *event) {
		auto levelBox = (dynamic_cast<LevelCrate *>(event->getCurrentTarget()));
		if (levelBox) {
			AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
			Director::getInstance()->pushScene(GameScene::create(levelBox->getLevelNum()));
		}
	};

	auto userData = manager->getUserData();
	char buf[8]; memset(buf, 0, sizeof(buf));
	
	for (int i = 0, crateIdx = 1; i < 3; ++i)
	{
		Layout *page = Layout::create();
		page->setContentSize(manager->VisibleSize);
		pageView->addPage(page);
		
		for (int row = 0; row < 6; ++row)
		{
			for (int col = 0; col < 5; ++col)
			{
				sprintf(buf, "%d", crateIdx);
				LevelCrate *lockCrate = LevelCrate::create(crateIdx, crateIdx > userData->getCurrentLevel());
				lockCrate->setPosition(pageLTPos + Vec2(col * offset.width, row * offset.height));
				lockCrate->setContentSize(crateSize);
				page->addChild(lockCrate);
				_crates.push_back(lockCrate);

				SpriteLabel *boxNumLabel = SpriteLabel::create(buf);
				boxNumLabel->setScale(.3f);
				boxNumLabel->setPosition(NODE_CENTER_POSITION(lockCrate));
				boxNumLabel->setVisible(!lockCrate->getLock());
				lockCrate->addChild(boxNumLabel);
				
				Director::getInstance()
					->getEventDispatcher()
					->addEventListenerWithSceneGraphPriority(shareTouchListener->clone(), lockCrate);

				if (++crateIdx > MAX_LEVEL)
				{
					return pageView;
				}
			}
		}
	}
	return pageView;
}
