
#pragma once

#ifndef __PLAYER_ELEMENT_H__
#define __PLAYER_ELEMENT_H__

#include "Level/MoveElement.h"

class PlayerElement : public MoveElement
{
public:
	static PlayerElement *create(ElementType type);
	virtual bool initWithType(ElementType type) override;
	virtual void setFaceDirection(FaceDirection faceDirection) override;
	virtual void move() override;

protected:
	struct PlayerStep : public Step {
		typedef std::function<void(cocos2d::Vec2, FaceDirection)> callbackFunc;
		callbackFunc callback;
		cocos2d::Vec2 position;
		FaceDirection faceDirection;
		PlayerStep(callbackFunc callback, cocos2d::Vec2 pos, FaceDirection faceDirection);
		virtual void call() override;
	};
	virtual MoveElement::Step *getStep() override;

private:
	cocos2d::SpriteFrameCache *_ccSpriteFrameCache;
};

#endif // !__PLAYER_ELEMENT_H__