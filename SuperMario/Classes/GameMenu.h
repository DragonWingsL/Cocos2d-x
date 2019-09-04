
#ifndef __GameMenu_H__
#define __GameMenu_H__

#include "Common.h"

class GameMenu : public Layer
{
public:
	static GameMenu* create()
	{
		GameMenu* menu = new GameMenu;
		menu->init();
		menu->autorelease();

		return menu;
	}

	void onEnter();
	void onExit();

	void Continue(Ref*);
	void Start(Ref*);
	void Retry(Ref*);

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);

	Menu* _menu; //保存菜单 用来处理菜单点击事件
	bool _bTouch;
};
#endif