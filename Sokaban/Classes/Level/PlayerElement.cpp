
#include <cocos2d.h>

#include "GlobalManager.h"
#include "Level/PlayerElement.h"

USING_NS_CC;

PlayerElement *PlayerElement::create(ElementType type)
{
	PlayerElement *pRet = new(std::nothrow) PlayerElement();
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

bool PlayerElement::initWithType(ElementType type)
{
	CCASSERT(!(type != ElementType::Player), "Can only create ElementType of Player.");

	do {
		CC_BREAK_IF(!MoveElement::init());
		_ccSpriteFrameCache = SpriteFrameCache::getInstance();

		// 默认面向前方
		_faceDirection = static_cast<FaceDirection>(-1);
		setFaceDirection(FaceDirection::Forward);
		setLocalZOrder(128);
		_type = type;
		return true;
	} while (0);
	return false;
}

void PlayerElement::setFaceDirection(FaceDirection faceDirection)
{
	MoveElement::setFaceDirection(faceDirection);

	// 更改贴图
	if (_faceDirection == FaceDirection::Forward) {
		setSpriteFrame(_ccSpriteFrameCache->getSpriteFrameByName("forward_0"));
	}
	else if (_faceDirection == FaceDirection::Backward){
		setSpriteFrame(_ccSpriteFrameCache->getSpriteFrameByName("backward_0"));
	}
	else if (_faceDirection == FaceDirection::Left) {
		setSpriteFrame(_ccSpriteFrameCache->getSpriteFrameByName("left_0"));
	}
	else {
		setSpriteFrame(_ccSpriteFrameCache->getSpriteFrameByName("right_0"));
	}
	setScale(.6f);
}

void PlayerElement::move()
{
	if (_isMoving)
		return;

	_steps.push(getStep());

	auto manager = GlobalManager::getInstance();
	Vec2 move = getNextMovePosition();
	Animate *animate = nullptr;

	if (_faceDirection == FaceDirection::Forward) {
		animate = manager->getAnimateForName(GlobalManager::PlayerAnimateName::Forward);
	}
	else if (_faceDirection == FaceDirection::Backward) {
		animate = manager->getAnimateForName(GlobalManager::PlayerAnimateName::Backward);
	}
	else if (_faceDirection == FaceDirection::Left) {
		animate = manager->getAnimateForName(GlobalManager::PlayerAnimateName::Left);
	}
	else {
		animate = manager->getAnimateForName(GlobalManager::PlayerAnimateName::Right);
	}

	MoveTo *moveAction = MoveTo::create(animate->getDuration(), move);
	Spawn *moveAndAnimAction = Spawn::create(moveAction, animate, nullptr);
	CallFunc *callAction = CallFunc::create([this]() {
		_isMoving = false;
		auto faceDirection = _faceDirection;
		_faceDirection = static_cast<FaceDirection>(-1);
		setFaceDirection(faceDirection);
	});
	Sequence *sequence = Sequence::create(moveAndAnimAction, callAction, nullptr);
	_isMoving = true;
	runAction(sequence);
}

MoveElement::Step *PlayerElement::getStep()
{
	PlayerStep *step = new PlayerStep([this](Vec2 lastPosition, FaceDirection lastFaceDirection) {
		setPosition(lastPosition);
		setFaceDirection(lastFaceDirection);
	}, getPosition(), getFaceDirection());
	return step;
}

PlayerElement::PlayerStep::PlayerStep(
	callbackFunc callback, cocos2d::Vec2 pos, FaceDirection faceDirection
) : callback(callback), position(pos), faceDirection(faceDirection)
{
}

void PlayerElement::PlayerStep::call()
{
	MoveElement::Step::call();
	callback(position, faceDirection);
}
