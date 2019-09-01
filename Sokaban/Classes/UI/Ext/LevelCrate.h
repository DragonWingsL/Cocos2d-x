
#pragma once

#ifndef __LEVEL_CRATE_H__
#define __LEVEL_CRATE_H__

class LevelCrate : public cocos2d::Sprite
{
public:
	static LevelCrate* create(int levelNum, bool isLock);

	CC_SYNTHESIZE_READONLY(int, _levelNum, LevelNum);
	CC_PROPERTY_PASS_BY_REF(bool, _isLock, Lock);

};

#endif // !__LEVEL_CRATE_H__
