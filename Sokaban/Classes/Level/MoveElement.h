
#pragma once

#ifndef __MOVE_ELEMENT_H__
#define __MOVE_ELEMENT_H__

#include "Level/LevelElement.h"

enum FaceDirection {
	Forward, Backward,
	Left, Right
};

class MoveElement : public LevelElement
{
public:
	virtual bool initWithType(ElementType type) override;
	virtual void move();
	virtual void setFaceDirection(FaceDirection faceDirection);
	FaceDirection getFaceDirection() const;
	cocos2d::Vec2 getNextMovePosition();
	bool isMoving() const;
	void back();

public:
	std::function<void(void)> onMoveEnded;

protected:
	struct Step {
		virtual void call() { };
	};

	virtual Step *getStep() = 0;

protected:
	FaceDirection _faceDirection;
	bool _isMoving;
	std::stack<Step *> _steps;
};

#endif // !__MOVE_ELEMENT_H__