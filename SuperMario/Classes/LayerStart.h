
#ifndef	__LayerStart_H__
#define	__LayerStart_H__

#include "Common.h"

class MyDataSource : public TableViewDataSource
{
public:
	MyDataSource()
	{
		_arrCells = __Array::create();
		_arrCells->retain();

		for (int i = 0; i < GAME_LEVEL; i++)
		{
			TableViewCell* cells = new TableViewCell();
			cells->init();
			_arrCells->addObject(cells);
			cells->autorelease();

			Sprite* levelMenu = Sprite::create(Util::format(i+1, "select", ".jpg"));
			levelMenu->setPosition(Vec2(winSize.width/2, winSize.height/2));
			levelMenu->setContentSize(winSize * .7f);
			cells->addChild(levelMenu);
		}
			

	}

	~MyDataSource()
	{
		_arrCells->release();
	}

	 virtual Size cellSizeForTable(TableView *table)
	 {
        return winSize;
    }

    virtual TableViewCell* tableCellAtIndex(TableView *table, unsigned int idx)
	{
		return (TableViewCell*)_arrCells->getObjectAtIndex(idx);
	}
 
    virtual ssize_t numberOfCellsInTableView(TableView *table)
	{
		return _arrCells->count();
	}

public:
	__Array* _arrCells;


	// 通过 TableViewDataSource 继承
	virtual TableViewCell *tableCellAtIndex(TableView * table, ssize_t idx) override;

};


class LayerStart : public Layer, public TableViewDelegate
{
public:
	CREATE_FUNC(LayerStart);

	void onEnter();
	void onExit();

	void initBackground();
	void initButton();
	void addScrollView();

	void Back(Ref*);

	//TableViewDelegate 的接口函数
	void tableCellTouched(TableView* table, TableViewCell* cell);

	//重载ScrollView接口函数
	virtual void scrollViewDidScroll(ScrollView* view){}
	virtual void scrollViewDidZoom(ScrollView* view) {}

	//成员变量
public:
	MyDataSource* _dataSource;

};

#endif