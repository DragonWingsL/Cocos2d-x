
#pragma once

#ifndef __LEVEL_DATA_H__
#define __LEVEL_DATA_H__

#include <unordered_map>
#include <vector>

#include "Level/Level.h"

class LevelData
{
public:
	LevelData();
	~LevelData();

public:
	static LevelData *getInstance();
	static void setDelegate(LevelData *delegate);

	Level *getLevel(int levelNum);

private:
	static LevelData *_inst;
	cocos2d::FileUtils *_ccFileUtils;

	rapidjson::Document _dataDoc;

	std::unordered_map<int, LEVEL_MAP> _levelCache;
};

#endif // !__LEVEL_DATA_H__