
#include <cocos2d.h>
#include <cocostudio/CocoStudio.h>

#include "CommonMacro.h"
#include "Level/LevelData.h"

USING_NS_CC;

LevelData *LevelData::_inst = nullptr;

LevelData::LevelData()
{
	_ccFileUtils = FileUtils::getInstance();
	do {
		
		Data data = _ccFileUtils->getDataFromFile("levels/level.json");
		_dataDoc.Parse((char *)data.getBytes(), data.getSize());
		CC_BREAK_IF(_dataDoc.HasParseError());
		_levelCache.clear();
	} while (0);
}

LevelData::~LevelData()
{
}

LevelData *LevelData::getInstance()
{
	return _inst;
}

void LevelData::setDelegate(LevelData *delegate)
{
	if (_inst)
		delete _inst;
	_inst = delegate;
}

Level *LevelData::getLevel(int levelNum)
{
	CCASSERT(levelNum > 0 && levelNum <= MAX_LEVEL, "Level number out of range.");

	char buf[16];
	sprintf(buf, "level-%d", levelNum);
	do {
		LEVEL_MAP levelData;

		auto target = _levelCache.find(levelNum);
		if (target != _levelCache.end())
			levelData = target->second;
		else {
			if (!_dataDoc["levels"].HasMember(buf)) {
				return nullptr;
			}

			using rapidjson::Value;
			const Value &obj = _dataDoc["levels"][buf];
			auto row = obj.GetArray();
			for (const auto &row : obj.GetArray()) {
				std::vector<int> rowData;
				for (const auto &col : row.GetArray()) {
					rowData.push_back(col.GetInt());
				}
				levelData.push_back(rowData);
			}
			_levelCache.insert(std::pair<int, LEVEL_MAP>(levelNum, levelData));
		}
		return Level::create(levelData);
	} while (0);
	return nullptr;
}
