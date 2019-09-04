#ifndef __SceneMenu_H__
#define __SceneMenu_H__

#include "Common.h"

class SceneMenu : public Layer
{
public:
	CREATE_FUNC(SceneMenu);
	bool init();

	void initScrollMenu();
	void Back(Ref*);

	virtual bool onTouchBegan(Touch*, Event*) override { return true; }
	virtual void onTouchEnded(Touch* t, Event*) override;

protected:
	void adjustScrollView(float);

	Node* _node;
};

#endif