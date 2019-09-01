
#pragma once

#ifndef __LEVEL_ELEMENT_H__
#define __LEVEL_ELEMENT_H__

enum ElementType {
	Wall,
	Floor,
	Crate,
	CrateTriggerOfEndPoint,
	Player,
	EndPoint
};

class LevelElement : public cocos2d::Sprite
{
public:
	static LevelElement *create(ElementType type);
	virtual bool initWithType(ElementType type);
	ElementType getType() const;
	void setLevelIdx(int idx);
	int getLevelIdx() const;

protected:
	ElementType _type;
	int _levelIdx;
};

#endif // !__LEVEL_ELEMENT_H__