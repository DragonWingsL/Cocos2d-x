
#include <cocos2d.h>

#include "CommonMacro.h"
#include "GlobalManager.h"
#include "Level/LevelElement.h"

USING_NS_CC;

LevelElement *LevelElement::create(ElementType type)
{
	LevelElement *pRet = new(std::nothrow) LevelElement();
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

bool LevelElement::initWithType(ElementType type)
{
	// Not support.
	// To be create a PlayerElement.
	CCASSERT(!(type == ElementType::Player), "Player type is not supported.");

	auto manager = GlobalManager::getInstance();
	do {
		std::string filename;
		if (type == ElementType::Wall) {
			filename = manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameLevelWall);
		}
		else if (type == ElementType::Floor) {
			filename = manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameLevelFloor);
		}
		else if (type == ElementType::Crate) {
			filename = manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameLevelCrateNormal);
		}
		else if (type == ElementType::CrateTriggerOfEndPoint) {
			filename = manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameLevelCrateTriggerOfEndPoint);
		}
		else if (type == ElementType::EndPoint) {
			filename = manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameLevelEndPoint);
		}

		CC_BREAK_IF(!Sprite::initWithFile(filename));
		_type = type;
		return true;
	} while (0);
	return false;
}

ElementType LevelElement::getType() const
{
	return _type;
}

void LevelElement::setLevelIdx(int idx)
{
	_levelIdx = idx;
}

int LevelElement::getLevelIdx() const
{
	return _levelIdx;
}
