
#pragma once

#ifndef __LOAD_RESOURCES_SCENE_H__
#define __LOAD_RESOURCES_SCENE_H__

class LoadResourceScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene *createScene();

	CREATE_FUNC(LoadResourceScene);

	virtual bool init() override;

	std::function<void(void)> onLoadCompleted;

	virtual void update(float delta) override;

private:
	bool IsLoadCompleted;
};

#endif // !__LOAD_RESOURCES_SCENE_H__