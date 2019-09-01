
#pragma once

#ifndef __SPRITE_LABEL_H__
#define __SPRITE_LABEL_H__

class SpriteLabel : public cocos2d::Node
{
	using Sprite = cocos2d::Sprite;
	using Vec2 = cocos2d::Vec2;
public:
	static SpriteLabel *create(std::string str);
	CREATE_FUNC(SpriteLabel);
	virtual bool init() override;
	virtual bool initWithString(std::string str);
	virtual void setPosition(const Vec2 &position) override;
	void setText(std::string str);
	cocos2d::Vector<Sprite *> &getLabelSprites();

protected:
	cocos2d::Vector<Sprite *> _sprites;
	cocos2d::Size _originalSize;
};

#endif // !__SPRITE_LABEL_H__