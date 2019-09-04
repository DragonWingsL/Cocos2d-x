
#include "MyScrollView.h"

MyScrollView* MyScrollView::create(TableViewDataSource* dataSource, Size size)
{
	MyScrollView *table = new MyScrollView;
	table->initWithViewSize(size, NULL);
	table->autorelease();
	table->setDataSource(dataSource);
	table->_updateCellPositions();
	table->_updateContentSize();
	
	return table;
}

void MyScrollView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!this->isVisible()) {
		return;
	}

	if (_touchedCell){
#if 0
		Rect bb = this->getBoundingBox();
		bb.origin = m_pParent->convertToWorldSpace(bb.origin);
#endif

		Sprite* sprite = (Sprite*)*(_touchedCell->getChildren().begin());
		Rect rcSprite = sprite->getBoundingBox();
		Vec2 ptInWorld = pTouch->getLocation();
		Vec2 ptInCell = _touchedCell->convertToNodeSpace(ptInWorld);
		
		if (rcSprite.containsPoint(ptInCell) && _tableViewDelegate != NULL)
		{
			_tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
			_tableViewDelegate->tableCellTouched(this, _touchedCell);
		}

		_touchedCell = NULL;
	}

	ScrollView::onTouchEnded(pTouch, pEvent);
}
