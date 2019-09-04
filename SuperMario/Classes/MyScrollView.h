
#ifndef	__MyScrollView_H__
#define	__MyScrollView_H__

#include "Common.h"

class MyScrollView : public TableView
{
public:
	static MyScrollView* create(TableViewDataSource* dataSource, Size size);

	void onTouchEnded(Touch *pTouch, Event *pEvent);

};

#endif