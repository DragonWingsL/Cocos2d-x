
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

	Menu* _menu; //����˵� ��������˵�����¼�
	bool _bTouch;
};
#endif