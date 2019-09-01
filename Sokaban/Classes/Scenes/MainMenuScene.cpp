
#include <cocos2d.h>
#include <algorithm>
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

#include "User/UserData.h"
#include "Audio/AudioUtils.h"
#include "LevelScene.h"
#include "CommonMacro.h"
#include "GlobalManager.h"
#include "MainMenuScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene *MainMenuScene::createScene()
{
	auto scene = MainMenuScene::create();
	return scene;
}

bool MainMenuScene::init()
{
	if (!Scene::init())
		return false;

	auto manager = GlobalManager::getInstance();
	Vec2 visibleOriginPos = manager->VisibleOriginPos;
	Vec2 centerPos = manager->VisibleCenterPos;
	Size visibleSize = manager->VisibleSize;

	Layer *defaultLayer = Layer::create();
	defaultLayer->setPosition(visibleOriginPos);
	addChild(defaultLayer);

	Sprite *background = Sprite::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuBackground));
	background->setPosition(centerPos);
	background->setContentSize(visibleSize);
	defaultLayer->addChild(background);

	Sprite *title = Sprite::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuTitle));
	title->setPosition(centerPos + Vec2(0, visibleSize.height * .3f));
	title->setScale(.7f);
	defaultLayer->addChild(title);

	Menu *featureMenu = Menu::create();
	addChild(featureMenu);

	MenuItem* playMenuItem = MenuItemImage::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuPlayBtn), "");
	playMenuItem->setCallback([](Ref *) {
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
		Director::getInstance()->pushScene(LevelScene::createScene());
	});
	playMenuItem->setScale(.5f);
	featureMenu->addChild(playMenuItem);

	Layer *infoLayer = Layer::create();
	infoLayer->setPosition(manager->VisibleOriginPos);
	infoLayer->setVisible(false);
	addChild(infoLayer, 3);
	
	Sprite *infoPanel = Sprite::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuInfo));
	infoPanel->setPosition(manager->VisibleCenterPos);
	infoLayer->addChild(infoPanel);

	Sprite *infoPanelCloseBtn = Sprite::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuInfoCloseBtnNormal));
	infoPanelCloseBtn->setPosition(manager->VisibleCenterPos + infoPanel->getContentSize() * ONE_OVER_TWO);
	infoPanelCloseBtn->setScale(.5f);
	infoLayer->addChild(infoPanelCloseBtn);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [infoPanelCloseBtn, infoLayer](Touch *touch, Event *event) -> bool {
		if (infoPanelCloseBtn->getBoundingBox().containsPoint(touch->getLocation())) {
			infoLayer->setVisible(false);
			return true;
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(listener, infoPanelCloseBtn);

	using namespace ui;
	Layout *layout = Layout::create();
	layout->setPosition(centerPos - Vec2(0, visibleSize.height * .1f));
	layout->setLayoutType(Layout::Type::HORIZONTAL);
	addChild(layout);

	Button *musicMenuItem = Button::create(
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuMusicBtnNormal), 
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuMusicBtnNormal),
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuMusicBtnClicked)
	);
	musicMenuItem->setBright(manager->IsEnableMusic);
	musicMenuItem->addClickEventListener([musicMenuItem](Ref *) {
		musicMenuItem->setBright(!musicMenuItem->isBright());
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
		GlobalManager::getInstance()->IsEnableMusic = musicMenuItem->isBright();
	});
	musicMenuItem->setScale(.5f);
	layout->addChild(musicMenuItem);

	Size smallBtnSize = musicMenuItem->getContentSize() * musicMenuItem->getScale();
	LinearLayoutParameter  *linearLayoutParam = LinearLayoutParameter::create();
	linearLayoutParam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	linearLayoutParam->setMargin(
		Margin(smallBtnSize.width * ONE_OVER_TWO, 0, 0, 0)
	);

	Button *soundMenuItem = Button::create(
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuSoundBtnNormal),
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuSoundBtnNormal),
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuSoundBtnClicked)
	);
	soundMenuItem->addClickEventListener([soundMenuItem](Ref *) {
		soundMenuItem->setBright(!soundMenuItem->isBright());
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
		GlobalManager::getInstance()->IsEnableSound = soundMenuItem->isBright();
	});
	soundMenuItem->setScale(.5f);
	soundMenuItem->setLayoutParameter(linearLayoutParam);
	layout->addChild(soundMenuItem);

	Button *infoMenuItem = Button::create(
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuInfoBtnNormal)
	);
	infoMenuItem->addClickEventListener([infoLayer](Ref *) {
		infoLayer->setVisible(true);
	});
	infoMenuItem->setScale(.5f);
	infoMenuItem->setLayoutParameter(linearLayoutParam);
	layout->addChild(infoMenuItem);

	layout->setPosition(layout->getPosition() - Vec2(
		(smallBtnSize.width * layout->getChildrenCount() + 
		smallBtnSize.width * ONE_OVER_TWO * (layout->getChildrenCount() - 1)
			) * ONE_OVER_TWO,
		0)
	);

	return true;
}