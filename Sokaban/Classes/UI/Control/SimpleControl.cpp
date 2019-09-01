
#include <cocos2d.h>

#include "CommonMacro.h"
#include "SimpleControl.h"

USING_NS_CC;

bool SimpleControl::init()
{
	auto manager = GlobalManager::getInstance();
	do {
		CC_BREAK_IF(!Sprite::initWithFile(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GameControl)));

		onButtonDown = nullptr; onButtonUp = nullptr;

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [=](Touch *touch, Event *event) -> bool {
			Vec2 arPos = this->convertToNodeSpaceAR(touch->getLocation());
			if (_up.containsPoint(arPos)) {
				if (onButtonDown != nullptr)
					onButtonDown(UP);
			}
			else if (_down.containsPoint(arPos)) {
				if (onButtonDown != nullptr)
					onButtonDown(DOWN);
			}
			else if (_left.containsPoint(arPos)) {
				if (onButtonDown != nullptr)
					onButtonDown(LEFT);
			}
			else if (_right.containsPoint(arPos)) {
				if (onButtonDown != nullptr)
					onButtonDown(RIGHT);
			}
			else return false;
			return true;
		};
		listener->onTouchEnded = [=](Touch *touch, Event *event) {
			Vec2 arPos = this->convertToNodeSpaceAR(touch->getLocation());
			if (_up.containsPoint(arPos)) {
				if (onButtonUp != nullptr) 
					onButtonUp(UP);
			}
			else if (_down.containsPoint(arPos)) {
				if (onButtonUp != nullptr)
					onButtonUp(DOWN);
			}
			else if (_left.containsPoint(arPos)) {
				if (onButtonUp != nullptr)
					onButtonUp(LEFT);
			}
			else if (_right.containsPoint(arPos)) {
				if (onButtonUp != nullptr)
					onButtonUp(RIGHT);
			}
			else return false;
			return true;
		};

		Director::getInstance()
			->getEventDispatcher()
			->addEventListenerWithSceneGraphPriority(listener, this);
		return true;
	} while (0);
	return false;
}

void SimpleControl::setScale(float scale)
{
	Sprite::setScale(scale);
	resetButtonAreas();
}

void SimpleControl::setPosition(const Vec2 & position)
{
	Sprite::setPosition(position);
	resetButtonAreas();
}

void SimpleControl::resetButtonAreas()
{
	Size realSize = getContentSize();
	Size averageOfNine = realSize / 3;
	Vec2 ltPos = Vec2(-realSize.width, realSize.height) * ONE_OVER_TWO;

	_up = Rect(ltPos + Vec2(1 * averageOfNine.width,	-1 * averageOfNine.height), averageOfNine);
	_down = Rect(ltPos + Vec2(1 * averageOfNine.width,  -3 * averageOfNine.height), averageOfNine);
	_left = Rect(ltPos + Vec2(0,						-2 * averageOfNine.height), averageOfNine);
	_right = Rect(ltPos + Vec2(2 * averageOfNine.width, -2 * averageOfNine.height), averageOfNine);
}

