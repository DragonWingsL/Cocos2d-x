
#include <cocos2d.h>
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

#include "UI/Label/SpriteLabel.h"
#include "UI/Control/SimpleControl.h"
#include "Level/LevelData.h"
#include "Level/Level.h"
#include "Level/CrateElement.h"
#include "Level/PlayerElement.h"
#include "Audio/AudioUtils.h"
#include "User/UserData.h"
#include "Scenes/LevelScene.h"
#include "CommonMacro.h"
#include "GlobalManager.h"
#include "GameScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

cocos2d::Scene *GameScene::create(int level)
{
	GameScene *pRet = new(std::nothrow) GameScene();
	if (pRet && pRet->initWithLevel(level))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool GameScene::init()
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
	
	auto texCache = Director::getInstance()->getTextureCache();

	Texture2D *texBackgroundTile = texCache->addImage(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameBackgroundTile));
	Texture2D::TexParams texParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	texBackgroundTile->setTexParameters(texParams);

	Sprite *background = Sprite::createWithTexture(
		texBackgroundTile, Rect(visibleOriginPos, visibleSize)
	);
	background->setPosition(centerPos);
	defaultLayer->addChild(background);

	initMenuPanel();

	_gameOverLayer = Layer::create();
	_gameOverLayer->setPosition(manager->VisibleOriginPos);
	_gameOverLayer->setVisible(false);
	addChild(_gameOverLayer, INT_MAX);

	Sprite *overlap = Sprite::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameOverlap));
	overlap->setPosition(manager->VisibleCenterPos);
	overlap->setContentSize(manager->VisibleSize);
	overlap->setOpacity(static_cast<GLubyte>(168));
	_gameOverLayer->addChild(overlap);

	using namespace ui;
	Layout *layout = Layout::create();
	layout->setLayoutType(Layout::Type::HORIZONTAL);
	_gameOverLayer->addChild(layout);

	Button *backBtn = Button::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameMenuBackBtnSmall));
	backBtn->setScale(.5f);
	backBtn->addClickEventListener([this](Ref *) {
		backToPrevScene();
	});
	layout->addChild(backBtn);

	Size BtnSize = backBtn->getContentSize() * .5f;

	Button *nextLevelBtn = Button::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameNextLevelBtnNormal));
	nextLevelBtn->setScale(.5f);
	LinearLayoutParameter *linearLayoutParamOfNextLevelBtn = LinearLayoutParameter::create();
	Size deltaSize = (nextLevelBtn->getContentSize() - backBtn->getContentSize()) * .5f;
	linearLayoutParamOfNextLevelBtn->setMargin(
		Margin(BtnSize.width * ONE_OVER_TWO, -deltaSize.height * ONE_OVER_TWO, 0, 0)
	);
	nextLevelBtn->setLayoutParameter(linearLayoutParamOfNextLevelBtn);
	nextLevelBtn->addClickEventListener([this](Ref *) {
		if (_isGameOver) {
			AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
			nextLevel();
		}
	});
	layout->addChild(nextLevelBtn);

	LinearLayoutParameter *linearLayoutParam = LinearLayoutParameter::create();
	linearLayoutParam->setMargin(
		Margin(BtnSize.width * ONE_OVER_TWO, 0, 0, 0)
	);

	Button *restartBtn = Button::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameMenuRestartBtnNormal));
	restartBtn->setScale(.5f);
	restartBtn->setLayoutParameter(linearLayoutParam);
	restartBtn->addClickEventListener([=](Ref *) {
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
		restart();
	});
	layout->addChild(restartBtn);

	layout->setContentSize(Size(BtnSize.width * layout->getChildrenCount() +
		BtnSize.width * ONE_OVER_TWO * (layout->getChildrenCount() - 1)
		, (nextLevelBtn->getContentSize() * .5f).height));

	layout->setPosition(manager->VisibleOriginPos + Vec2(manager->VisibleCenterPos.x
		- (BtnSize.width * layout->getChildrenCount() +
			BtnSize.width * ONE_OVER_TWO * (layout->getChildrenCount() - 1)
			) * ONE_OVER_TWO,
		manager->VisibleSize.height * .3f));

	std::stringstream ss;
	ss << "level " << _levelNum;
	_levelNumLabel = SpriteLabel::create(ss.str());
	_levelNumLabel->setScale(.5f);
	Size s = _levelNumLabel->getContentSize();
	_levelNumLabel->setPosition(Vec2(
		manager->VisibleCenterPos.x,
		manager->VisibleCenterPos.y + _levelNumLabel->getContentSize().height * .5f * ONE_OVER_TWO
	)
	);
	_gameOverLayer->addChild(_levelNumLabel);

	SpriteLabel *gameCompletedLabel = SpriteLabel::create("completed");
	gameCompletedLabel->setScale(.5f);
	gameCompletedLabel->setPosition(Vec2(
		manager->VisibleCenterPos.x,
		manager->VisibleCenterPos.y - gameCompletedLabel->getContentSize().height * .5f * ONE_OVER_TWO
	)
	);
	_gameOverLayer->addChild(gameCompletedLabel);

	_swallowListener = EventListenerTouchOneByOne::create();
	_swallowListener->setSwallowTouches(true);
	_swallowListener->onTouchBegan = [=](Touch *touch, Event *event) -> bool {
		if (_isGameOver) {
			if (!layout->getBoundingBox().containsPoint(touch->getLocation())) {
				return true;
			}
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_swallowListener, -128);

	return true;
}

void GameScene::initMenuPanel()
{
	auto manager = GlobalManager::getInstance();
	Vec2 visibleOriginPos = manager->VisibleOriginPos;
	Vec2 centerPos = manager->VisibleCenterPos;
	Size visibleSize = manager->VisibleSize;

	Layer *menuLayer = Layer::create();
	menuLayer->setPosition(manager->VisibleOriginPos);
	addChild(menuLayer, 2);

	Sprite *menuBackground = Sprite::create(
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameMenuBackground)
	);
	menuBackground->setContentSize(Size(visibleSize.width, visibleSize.height * .15f));
	menuBackground->setPosition(visibleOriginPos +
		Vec2(visibleSize.width * ONE_OVER_TWO,
			menuBackground->getContentSize().height * ONE_OVER_TWO)
	);
	_levelLeftBottomPos = Vec2(
		0, 
		menuBackground->getPosition().y + menuBackground->getContentSize().height * ONE_OVER_TWO
	);
	menuLayer->addChild(menuBackground);

	using namespace ui;
	Layout *menuLayout = Layout::create();
	menuLayout->setLayoutType(Layout::Type::HORIZONTAL);
	menuLayer->addChild(menuLayout);
		
	Button *musicMenuItem = Button::create(
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuMusicBtnNormal),"",
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::MainMenuMusicBtnClicked)
	);
	musicMenuItem->setScale(.5f);
	musicMenuItem->setPosition(
		manager->VisibleOriginPos + manager->VisibleSize - 
		musicMenuItem->getContentSize() * musicMenuItem->getScale()
	);
	musicMenuItem->setBright(manager->IsEnableMusic);
	musicMenuItem->addClickEventListener([musicMenuItem](Ref *) {
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
		GlobalManager::getInstance()->IsEnableMusic = !GlobalManager::getInstance()->IsEnableMusic;
		musicMenuItem->setBright(GlobalManager::getInstance()->IsEnableMusic);
	});
	menuLayer->addChild(musicMenuItem);
	
	Button *repentMenuItem = Button::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameMenuRepentBtnNormal));
	repentMenuItem->setScale(.5f);
	repentMenuItem->addClickEventListener([this](Ref *) {
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
		repent();
	});
	menuLayout->addChild(repentMenuItem);

	Size smallBtnSize = repentMenuItem->getContentSize() * repentMenuItem->getScale();
	LinearLayoutParameter *linearLayoutParam = LinearLayoutParameter::create();
	linearLayoutParam->setMargin(
		Margin(smallBtnSize.width * ONE_OVER_TWO, 0, 0, 0)
	);
	
	LinearLayoutParameter *linearLayoutParamOfBackBtn = LinearLayoutParameter::create();
	
	Button *backLevelMenuItem = Button::create(
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameMenuBackBtnMiddle),"",
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameMenuBackBtnMiddle)
	);
	linearLayoutParamOfBackBtn->setMargin(
		Margin(smallBtnSize.width * ONE_OVER_TWO, -backLevelMenuItem->getContentSize().height * .2f * ONE_OVER_TWO, 0, 0)
	);
	backLevelMenuItem->addClickEventListener([this](Ref *) {
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
		backToPrevScene();
	});
	backLevelMenuItem->setScale(.7f);
	backLevelMenuItem->setLayoutParameter(linearLayoutParamOfBackBtn);
	menuLayout->addChild(backLevelMenuItem);

	Button *restartMenuItem = Button::create(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameMenuRestartBtnNormal));
	restartMenuItem->setScale(.5f);
	restartMenuItem->setLayoutParameter(linearLayoutParam);
	restartMenuItem->addClickEventListener([this](Ref *) {
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Touch);
		restart();
	});
	menuLayout->addChild(restartMenuItem);

	menuLayout->setPosition(manager->VisibleOriginPos + Vec2(manager->VisibleCenterPos.x 
		-(smallBtnSize.width * menuLayout->getChildrenCount() +
			smallBtnSize.width * ONE_OVER_TWO * (menuLayout->getChildrenCount() - 1)
			) * ONE_OVER_TWO,
			(menuBackground->getContentSize().height + smallBtnSize.height) * ONE_OVER_TWO)
	);
}

bool GameScene::initWithLevel(int levelNum)
{
	_levelNum = levelNum;
	if (!GameScene::init())
		return false;

	auto manager = GlobalManager::getInstance();
	_level = manager->getLevelData()->getLevel(_levelNum);
	_level->setPosition(_level->getPosition() + _levelLeftBottomPos);
	_endpointTriggerCount = _level->getDefaultEndpointTriggerCount();
	addChild(_level, 1);

	if (_endpointTriggerCount > 0) {
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Drop);
	}

	Layer *controlLayer = Layer::create();
	controlLayer->setPosition(manager->VisibleOriginPos);
	addChild(controlLayer, 2);

	SimpleControl *control = SimpleControl::create();
	control->setScale(.7f);
	control->setPosition(_levelLeftBottomPos + control->getContentSize() * control->getScale() * ONE_OVER_TWO);
	controlLayer->addChild(control);
	
	control->onButtonDown = [=](SimpleControl::ButtonStatus buttonStatus) {
		auto player = _level->getPlayer();
		if (player->isMoving())
			return;

		LevelElement *front = nullptr;
		if (buttonStatus == SimpleControl::ButtonStatus::UP) {
			player->setFaceDirection(FaceDirection::Forward);
		}
		else if (buttonStatus == SimpleControl::ButtonStatus::DOWN) {
			player->setFaceDirection(FaceDirection::Backward);
		}
		else if (buttonStatus == SimpleControl::ButtonStatus::LEFT) {
			player->setFaceDirection(FaceDirection::Left);
		}
		else if (buttonStatus == SimpleControl::ButtonStatus::RIGHT) {
			player->setFaceDirection(FaceDirection::Right);
		}
		Vec2 v = Vec2(player->getNextMovePosition());
		front = _level->getElementByPosition(v);

		if (front->getType() == ElementType::Wall)
			return;

		Repent repent;

		if (front->getType() == ElementType::Crate || 
			front->getType() == ElementType::CrateTriggerOfEndPoint) {
			CrateElement *crate = (CrateElement *)front;
			crate->setFaceDirection(player->getFaceDirection());
			v = Vec2(crate->getNextMovePosition());
			LevelElement *frontOfCrate = _level->getElementByPosition(v);

			if (frontOfCrate->getType() == ElementType::Wall || 
				frontOfCrate->getType() == ElementType::Crate ||
				frontOfCrate->getType() == ElementType::CrateTriggerOfEndPoint)
				return;

			crate->onMoveEnded = [=]() {
				Vec2 pos = crate->getPosition();
				int newIdx = _level->convertPositionToLevelIdx(pos);
				_level->resetIdxOfElement(newIdx, crate);
				if (frontOfCrate->getType() == ElementType::EndPoint) {
					if (crate->getStatus() != CrateStatus::Trigger) {
						++_endpointTriggerCount;
						crate->setStatus(CrateStatus::Trigger);
						if (_endpointTriggerCount >= _level->getEndpointCount()) {
							onGameOver();
						}
					}
				}
				else if (crate->getStatus() == CrateStatus::Trigger){
					crate->setStatus(CrateStatus::Normal);
					--_endpointTriggerCount;
				}
			};
			AudioUtils::getInstance()->play(AudioUtils::AudioName::Crate);
			crate->move();
			repent.add(crate);
		}

		// move the level.
		if (player->getFaceDirection() == FaceDirection::Left) {
			_level->move(true);
		}
		else if (player->getFaceDirection() == FaceDirection::Right) {
			_level->move(false);
		}

		player->move();
		repent.add(player);
		_repentStack.push(repent);
	};

	control->onButtonUp = [=](SimpleControl::ButtonStatus buttonStatus) {
		if (buttonStatus == SimpleControl::ButtonStatus::UP) {
		}
		else if (buttonStatus == SimpleControl::ButtonStatus::DOWN) {
		}
		else if (buttonStatus == SimpleControl::ButtonStatus::LEFT) {
		}
		else if (buttonStatus == SimpleControl::ButtonStatus::RIGHT) {
		}
	};
	return true;
}

void GameScene::onGameOver()
{
	AudioUtils::getInstance()->play(AudioUtils::AudioName::Completed);
	_isGameOver = true;
	_gameOverLayer->setVisible(true);
	auto userData = GlobalManager::getInstance()->getUserData();
	userData->setCurrentLevel(_levelNum + 1);
	userData->save();
}

void GameScene::nextLevel()
{
	auto manager = GlobalManager::getInstance();
	_isGameOver = false;
	_gameOverLayer->setVisible(false);
	removeChild(_level);
	++_levelNum;
	_level = manager->getLevelData()->getLevel(_levelNum);
	_level->setPosition(_level->getPosition() + _levelLeftBottomPos);
	_endpointTriggerCount = _level->getDefaultEndpointTriggerCount();
	addChild(_level, 1);

	if (_endpointTriggerCount > 0) {
		AudioUtils::getInstance()->play(AudioUtils::AudioName::Drop);
	}

	std::stringstream ss;
	ss << "level " << _levelNum;
	_levelNumLabel->setText(ss.str());
}

void GameScene::restart()
{
	_isGameOver = false;
	_gameOverLayer->setVisible(false);
	int c = _repentStack.size();
	while (c--) {
		repent();
	}
}

void GameScene::repent()
{
	if (_repentStack.size() <= 0)
		return;

	_repentStack.top().restore();
	_repentStack.pop();
}

Level *GameScene::getCurrentLevel() const
{
	return _level;
}

void GameScene::backToPrevScene()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(_swallowListener);
	Director::getInstance()->popScene();
	_exitSceneListener = EventListenerCustom::create(Director::EVENT_AFTER_SET_NEXT_SCENE,
		[=](EventCustom *) {
		((LevelScene *)Director::getInstance()->getRunningScene())->updateCrateLock();
		Director::getInstance()->getEventDispatcher()->removeEventListener(_exitSceneListener);
		release();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_exitSceneListener, 1);
	retain();
}

void GameScene::Repent::add(MoveElement * element)
{
	elements.pushBack(element);
}

void GameScene::Repent::restore()
{
	GameScene *gameScene = ((GameScene *)Director::getInstance()->getRunningScene());
	CrateStatus status = CrateStatus::Normal;
	for (const auto &e : elements) {
		CrateElement *crate = dynamic_cast<CrateElement *>(e);
		if (crate) {
			status = crate->getStatus();
			e->back();
			if (status != crate->getStatus()) {
				if (status == CrateStatus::Normal) {

					++gameScene->_endpointTriggerCount;
				}
				else {
					--gameScene->_endpointTriggerCount;
				}
			}
		}
		else
			e->back();
	}
}
