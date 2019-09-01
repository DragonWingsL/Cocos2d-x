
#include <cocos2d.h>

#include "Level/Level.h"
#include "Scenes/GameScene.h"
#include "CommonMacro.h"
#include "Level/CrateElement.h"

USING_NS_CC;

CrateElement *CrateElement::create(ElementType type)
{
	CrateElement *pRet = new(std::nothrow) CrateElement();
	if (pRet && pRet->initWithType(type))
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

bool CrateElement::initWithType(ElementType type)
{
	do {
		CC_BREAK_IF(!MoveElement::initWithType(type));
		_ccTexCache = Director::getInstance()->getTextureCache();
		_status = static_cast<CrateStatus>(-1);
		setStatus(CrateStatus::Normal);
		return true;
	} while (0);
	return false;
}

void CrateElement::setStatus(CrateStatus status)
{
	if (_status == status)
		return;

	auto manager = GlobalManager::getInstance();
	_status = status;

	if (_status == CrateStatus::Normal) {
		setTexture(_ccTexCache->getTextureForKey(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameLevelCrateNormal)));
		_type = ElementType::Crate;
	}
	else {
		setTexture(_ccTexCache->getTextureForKey(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameLevelCrateTriggerOfEndPoint)));
		_type = ElementType::CrateTriggerOfEndPoint;
	}
	setScale(.6f);
}

CrateStatus CrateElement::getStatus() const
{
	return _status;
}

MoveElement::Step *CrateElement::getStep()
{
	CrateStep *step = new CrateStep([this](Vec2 lastPosition, CrateStatus lastCrateStatus) {
		setStatus(lastCrateStatus);
		setPosition(lastPosition);
		GameScene *gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
		Level *level = gameScene->getCurrentLevel();
		level->resetIdxOfElement(level->convertPositionToLevelIdx(lastPosition), this);
	}, getPosition(), getStatus());
	return step;
}

CrateElement::CrateStep::CrateStep(callbackFunc callback, cocos2d::Vec2 pos, CrateStatus crateStatus)
	: callback(callback), position(pos), crateStatus(crateStatus)
{
}

void CrateElement::CrateStep::call()
{
	MoveElement::Step::call();
	callback(position, crateStatus);
}
