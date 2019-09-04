
#ifndef __LayerSetting_H__
#define __LayerSetting_H__

#include "Common.h"

class LayerSetting : public Layer
{
public:
	CREATE_FUNC(LayerSetting);

	void onEnter();
	void onExit();

	void Back(Ref*);
	void Effect(Ref* , Control::EventType);
	void Music(Ref* , Control::EventType);

};
#endif