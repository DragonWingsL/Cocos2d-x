#ifndef __SceneGame_H__
#define __SceneGame_H__

#include "Common.h"

class Hero;
class AI;

class SceneGame : public Layer
{
public:

	static SceneGame* create(int planeID)
	{
		SceneGame* game = new SceneGame();
		game->init(planeID);
		game->autorelease();
		return game;
	}

	bool init(int planeID);

	void Back(Ref*);
	void addHeroPlane(int id);
	void createEnemyAI();
	void addLabels();
	
	bool onTouchBegan(Touch* t, Event*)
	{ 
		// 点中飞机才能移动
		//return _hero->getBoundingBox().containsPoint(t->getLocation());
		return true;
	}
	void onTouchMoved(Touch* t, Event*);

	void update(float dt);

protected:
	Hero* _hero;
	AI* _ai;

	LabelAtlas* _score;
	LabelAtlas* _hp;
};

#endif