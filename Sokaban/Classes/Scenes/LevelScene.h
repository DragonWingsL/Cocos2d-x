
#pragma once

#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "UI/Ext/LevelCrate.h"

class LevelScene : public cocos2d::Scene
{
public:
	CREATE_FUNC(LevelScene);

	static cocos2d::Scene *createScene();

	virtual bool init() override;
	void updateCrateLock();

private:
	cocos2d::ui::PageView *initLevelCratePageView();
	std::vector<LevelCrate *> _crates;
};

#endif // !__LEVEL_SCENE_H__