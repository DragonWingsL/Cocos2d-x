
#include <cocos2d.h>

#include "GlobalManager.h"
#include "MoveElement.h"

USING_NS_CC;

bool MoveElement::initWithType(ElementType type)
{
	onMoveEnded = nullptr;
	return LevelElement::initWithType(type);
}

void MoveElement::move()
{
	if (_isMoving)
		return;

	_steps.push(getStep());

	auto manager = GlobalManager::getInstance();
	Vec2 move = getNextMovePosition();

	MoveTo *moveAction = MoveTo::create(.5f, move);
	CallFunc *callAction = CallFunc::create([this]() {
		_isMoving = false;
		auto faceDirection = _faceDirection;
		_faceDirection = static_cast<FaceDirection>(-1);
		setFaceDirection(faceDirection);
		if (onMoveEnded)
			onMoveEnded();
	});
	Sequence *sequence = Sequence::create(moveAction, callAction, nullptr);
	_isMoving = true;
	runAction(sequence);
}

void MoveElement::setFaceDirection(FaceDirection faceDirection)
{
	if (_faceDirection == faceDirection)
		return;
	_faceDirection = faceDirection;
}

FaceDirection MoveElement::getFaceDirection() const
{
	return _faceDirection;
}

cocos2d::Vec2 MoveElement::getNextMovePosition()
{
	auto manager = GlobalManager::getInstance();
	Vec2 v = getPosition();
	if (_faceDirection == FaceDirection::Forward) {
		v.add(Vec2(0, manager->CellSize.height));
	}
	else if (_faceDirection == FaceDirection::Backward) {
		v.add(Vec2(0, -manager->CellSize.height));
	}
	else if (_faceDirection == FaceDirection::Left) {
		v.add(Vec2(-manager->CellSize.width, 0));
	}
	else if (_faceDirection == FaceDirection::Right) {
		v.add(Vec2(manager->CellSize.width, 0));
	}

	return (v);
}

bool MoveElement::isMoving() const
{
	return _isMoving;
}

void MoveElement::back()
{
	if (_steps.size() <= 0)
		return;

	Step *step = _steps.top();
	step->call();
	_steps.pop();
	delete step;
}

