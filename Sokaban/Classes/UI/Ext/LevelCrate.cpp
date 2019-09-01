
#include <cocos2d.h>

#include "CommonMacro.h"
#include "LevelCrate.h"

USING_NS_CC;

LevelCrate* LevelCrate::create(int levelNum, bool isLock)
{
	auto manager = GlobalManager::getInstance();
	LevelCrate *levelCrate = new (std::nothrow) LevelCrate();
	do {
		CC_BREAK_IF(!levelCrate);
		levelCrate->_isLock = isLock;
		if (levelCrate->_isLock) {
			CC_BREAK_IF(!levelCrate->initWithFile(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelLockCrate)));
		}
		else {
			CC_BREAK_IF(!levelCrate->initWithFile(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelUnlockCrate)));
		}
		levelCrate->_levelNum = levelNum;
		levelCrate->autorelease();
		return levelCrate;
	} while (0);
	CC_SAFE_DELETE(levelCrate);
	return nullptr;
}

void LevelCrate::setLock(const bool &isLock)
{
	if (_isLock == isLock)
		return;

	auto manager = GlobalManager::getInstance();

	_isLock = isLock;
	Size size = getContentSize();
	if (_isLock) {
		setTexture(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelLockCrate));
	}
	else {
		setTexture(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::LevelUnlockCrate));
	}
	setContentSize(size);

	for (const auto &child : getChildren()) {
		child->setVisible(!_isLock);
	}
}

const bool &LevelCrate::getLock() const
{
	return _isLock;
}