
#pragma once

#ifndef __SIMPLE_CONTROL_H__
#define __SIMPLE_CONTROL_H__

class SimpleControl : public cocos2d::Sprite
{
public:
	enum ButtonStatus{
		UP, DOWN, LEFT, RIGHT
	};
public:
	CREATE_FUNC(SimpleControl);
	virtual bool init() override;
	virtual void setScale(float scale) override;
	virtual void setPosition(const cocos2d::Vec2 &position) override;

protected:
	void resetButtonAreas();

public:
	typedef std::function<void(ButtonStatus)> ControlCallback;

	ControlCallback onButtonDown;
	ControlCallback onButtonUp;

protected:
	cocos2d::Rect _up;
	cocos2d::Rect _down;
	cocos2d::Rect _left;
	cocos2d::Rect _right;
};

#endif // !__SIMPLE_CONTROL_H__