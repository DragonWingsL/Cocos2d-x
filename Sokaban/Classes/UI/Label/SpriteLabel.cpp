
#include <cocos2d.h>

#include "CommonMacro.h"
#include "GlobalManager.h"
#include "SpriteLabel.h"

USING_NS_CC;

SpriteLabel *SpriteLabel::create(std::string str)
{
	SpriteLabel *pRet = new(std::nothrow) SpriteLabel();
	if (pRet && pRet->initWithString(str))
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

bool SpriteLabel::init()
{
	if (!Node::init())
		return false;

	return true;
}

bool SpriteLabel::initWithString(std::string str)
{
	if (!Node::init())
		return false;
	_originalSize = Size::ZERO;
	setText(str);
	return true;
}

void SpriteLabel::setPosition(const Vec2 &position)
{
	Node::setPosition(position);

	int offset = 0;
	Size s = getContentSize();
	Vec2 startPos = -Vec2(s.width * ONE_OVER_TWO - 
		s.width / float(_sprites.size()) * ONE_OVER_TWO, 0);
	for (const auto &sprite : _sprites) {
		sprite->setPosition(startPos + Vec2(offset, 0));
		offset += sprite->getContentSize().width;
	}
}

void SpriteLabel::setText(std::string str)
{
	removeAllChildren();
	_sprites.clear();
	setContentSize(_originalSize);
	auto manager = GlobalManager::getInstance();
	auto texCache = Director::getInstance()->getTextureCache();
	auto texAbcFontSet = texCache->getTextureForKey(
		manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::AbcFontSetImage)
	);
	for (const auto &c : str) {
		auto sprite = Sprite::createWithTexture(
			texAbcFontSet,
			manager->getAbcFontSetRectangleByChar(toupper(c))
		);
		setContentSize(getContentSize() + Size(sprite->getContentSize().width, 0));
		_sprites.pushBack(sprite);
		addChild(sprite);
	}
	Size s = getContentSize();
	s.height = (*getChildren().begin())->getContentSize().height;
	setContentSize(s);
	setPosition(getPosition());
}

cocos2d::Vector<Sprite*> &SpriteLabel::getLabelSprites()
{
	return _sprites;
}
