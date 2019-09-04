
#ifndef __LayerMenu_H__
#define	__LayerMenu_H__

#include "Common.h"

class LayerMenu : public Layer
{
public:
	static Scene* scene();
	CREATE_FUNC(LayerMenu);
	bool init();

	void initBackground();
	void initButton();

	void Start(Ref*);
	void Setting(Ref*);
	void Quit(Ref*);
	void About(Ref*);


};


#endif