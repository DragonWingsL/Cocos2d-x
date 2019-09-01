
#include <cocos2d.h>
#include <SimpleAudioEngine.h>
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

#include "Audio/AudioUtils.h"
#include "CommonMacro.h"
#include "GlobalManager.h"
#include "LoadResourceScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene *LoadResourceScene::createScene()
{
	return LoadResourceScene::create();
}

bool LoadResourceScene::init()
{
	if (!Scene::init())
		return false;

	IsLoadCompleted = false;

	auto manager = GlobalManager::getInstance();
	Vec2 visibleOriginPos = manager->VisibleOriginPos;
	Vec2 centerPos = manager->VisibleCenterPos;
	Size visibleSize = manager->VisibleSize;

	Layer *defaultLayer = Layer::create();
	defaultLayer->setPosition(visibleOriginPos);
	addChild(defaultLayer);

	using ui::LoadingBar;
	ui::LoadingBar *loadingBar = LoadingBar::create("images/b_control-sheet0.png", .0f);
	loadingBar->setPosition(centerPos);
	loadingBar->ignoreContentAdaptWithSize(false);
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	loadingBar->setContentSize(Size(visibleSize.width * .8f, visibleSize.height * .03f));
	defaultLayer->addChild(loadingBar);
	
	Label *loadingPercLabel = Label::create("0%", "Arial", 30.0f);
	loadingPercLabel->setAnchorPoint(Vec2(.5f, .5f));
	loadingPercLabel->setPosition(loadingBar->getPosition() +
		Vec2(0, 
			loadingBar->getContentSize().height * ONE_OVER_TWO + 
			loadingPercLabel->getContentSize().height * ONE_OVER_TWO
		)
	);
	defaultLayer->addChild(loadingPercLabel);

	auto ccSimpleAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	auto textureCache = Director::getInstance()->getTextureCache();
	auto loadingResources = GlobalManager::getInstance()->getSpriteResourcesPathRecords();
	auto preloadAudioResources = AudioUtils::getInstance()->getAudioPreloadResources();

	int total = loadingResources.size() + preloadAudioResources.size();
	float percOfPair = 100.0f / total;
	char buf[8];
	memset(buf, 0, sizeof(buf));
	for (const auto &audio : preloadAudioResources) {
		ccSimpleAudioEngine->preloadEffect(audio.second.c_str());
		loadingBar->setPercent(loadingBar->getPercent() + percOfPair);
		sprintf(buf, "%d%%", (int)loadingBar->getPercent());
		loadingPercLabel->setString(buf);
	}

	for (auto &pair : loadingResources) {
		textureCache->addImageAsync(pair.second,
			[=](Texture2D *) {
			static int counter = AudioUtils::getInstance()->getAudioPreloadResources().size();
			memset((void *)buf, 0, sizeof(buf));
			if (++counter >= total) {
				loadingBar->setPercent(100.0f);
				IsLoadCompleted = true;
				// 启用更新(update)
				scheduleUpdate();
			}
			else {
				loadingBar->setPercent(loadingBar->getPercent() + percOfPair);
			}
			sprintf((char *)buf, "%d%%", (int)loadingBar->getPercent());
			loadingPercLabel->setString(buf);
		});
	}

	return true;
}

void LoadResourceScene::update(float delta)
{
	if (IsLoadCompleted) {
		// 停用更新(update)
		unscheduleUpdate();
		onLoadCompleted();
	}
}
