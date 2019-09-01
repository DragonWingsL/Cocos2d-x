
#pragma once

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include <stack>

#include "Level/MoveElement.h"

class Level;
class SpriteLabel;

class GameScene : public cocos2d::Scene
{
private:
	struct Repent {
		cocos2d::Vector<MoveElement *> elements;
		void add(MoveElement *element);
		void restore();
	};

public:
	static cocos2d::Scene *create(int level);
	virtual bool init() override;
	bool initWithLevel(int level);
	void onGameOver();
	void restart();
	void repent();
	Level *getCurrentLevel() const;
	void backToPrevScene();

private:
	// 初始化游戏菜单面板
	void initMenuPanel();
	void nextLevel();

protected:
	int _levelNum;
	cocos2d::Vec2 _levelLeftBottomPos;
	cocos2d::Layer *_gameOverLayer;
	int _endpointTriggerCount;
	bool _isGameOver;
	Level *_level;
	SpriteLabel *_levelNumLabel;
	cocos2d::EventListenerTouchOneByOne  *_swallowListener;
	std::stack<Repent> _repentStack;
	cocos2d::EventListener *_exitSceneListener;
};

#endif // !__GAME_SCENE_H__