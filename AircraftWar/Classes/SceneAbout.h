#ifndef __SceneAbout_H__
#define __SceneAbout_H__

#include "Common.h"

class SceneAbout : public Layer
{
public:
	CREATE_FUNC(SceneAbout);
	bool init();

	void Back(Ref*);
	void setInfo();
};

#endif