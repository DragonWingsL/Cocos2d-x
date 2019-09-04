
#ifndef	__LayerAbout_H__
#define	__LayerAbout_H__

#include "Common.h"

class LayerAbout : public Layer
{
public:
	CREATE_FUNC(LayerAbout);
	bool init();

	void Back(Ref*);

	LabelTTF* _ttf;
};




#endif