
#pragma once

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <unordered_map>
#include <vector>

#include "Level/LevelElement.h"

#define IS_EMPTY(__flag__) (__flag__ == -1)

typedef std::vector<std::vector<int>> LEVEL_MAP;
class PlayerElement;

class Level : public cocos2d::Layer
{
public:
	Level(LEVEL_MAP &map);
	~Level();

public:
	static Level *create(LEVEL_MAP &map);
	bool initWithMap(LEVEL_MAP &map);
	PlayerElement *getPlayer() const;
	LevelElement *getElementByPosition(cocos2d::Vec2 &pos) const;
	ssize_t getHorizontalCellCount() const;
	int convertPositionToLevelIdx(cocos2d::Vec2 &pos) const;
	void resetIdxOfElement(int idx, LevelElement *element);
	ssize_t getEndpointCount() const;
	ssize_t getDefaultEndpointTriggerCount() const;
	void move(bool isLeft);

protected:
	LevelElement *spawn(ElementType type, cocos2d::Vec2 &pos);
	void addElement(int idx, LevelElement *element, int zOrder = 0, bool isAdd = true);

protected:
	LEVEL_MAP _map;
	PlayerElement *_player;
	std::unordered_map<int, std::vector<LevelElement *>> _idxOfElements;

	ssize_t _horiCellCount;
	ssize_t _endpointCount;
	ssize_t _endpointTriggerCount;

	int _curMark;
	int _leftMark;
	int _rightMark;
};

#endif // !__LEVEL_H__