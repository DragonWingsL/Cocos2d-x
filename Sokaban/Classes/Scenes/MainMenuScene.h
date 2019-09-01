
#pragma once

#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

class MainMenuScene : public cocos2d::Scene
{
public:
	CREATE_FUNC(MainMenuScene);
	static cocos2d::Scene *createScene();
	virtual bool init() override;
};

#endif // !__MAIN_MENU_SCENE_H__
