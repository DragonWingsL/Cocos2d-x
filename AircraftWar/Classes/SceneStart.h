#ifndef __SceneStart_H__
#define __SceneStart_H__

#include "Common.h"

class SceneStart : public Layer
{
public:
	CREATE_FUNC(SceneStart);
	bool init();

	void Start(Ref* sender);
	void About(Ref* sender);
	void Quit(Ref* sender);
};

#endif