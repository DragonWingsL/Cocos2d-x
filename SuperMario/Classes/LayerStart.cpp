
#include "LayerStart.h"
#include "LayerMenu.h"
#include "MyScrollView.h"
#include "LayerGame.h"

void LayerStart::onEnter()
{
	Layer::onEnter();
	setPosition(originPos);
	//初始化背景
	initBackground();
	//添加滚动菜单
	addScrollView();
	//添加返回按钮
	initButton();
}

void LayerStart::onExit()
{
	Layer::onExit();
	delete _dataSource;
}

void LayerStart::addScrollView()
{
	_dataSource = new MyDataSource();
	MyScrollView* scrollView = MyScrollView::create(_dataSource, winSize);
	CCLOG("winSize.width=%d, winSize.height=%d",winSize.width, winSize.height);
	addChild(scrollView);
	//设置水平方向 
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL); //要放在处理的上面 不然会导致图片消失

	scrollView->reloadData();
	//设置代理 处理点击事件
	scrollView->setDelegate(this);

}

void LayerStart::initBackground()
{
	Util::addBackground(IMAGE_START_bg, this);
}

void LayerStart::initButton()
{
	MenuItem* itemBack = MenuItemImage::create(IMAGE_START_back_1, IMAGE_START_back_2, this, menu_selector(LayerStart::Back));
	Menu* menu = Menu::create(itemBack, NULL);
	addChild(menu);
	itemBack->setScale(2.0f);
	Size realSize(itemBack->getContentSize() * itemBack->getScale());
	float x = winSize.width  /2 - realSize.width / 2;
	float y = winSize.height / 2 - realSize.height / 2;
	Util::moveNode(itemBack, Vec2(x, -y));
}

void LayerStart::Back(Ref*)
{
	switchScene(LayerMenu);
}

void LayerStart::tableCellTouched(TableView* table, TableViewCell* cell)
{
	//获得点击的level
	int index = cell->getIdx();
	switchScene(LayerGame, index);
}

TableViewCell *MyDataSource::tableCellAtIndex(TableView * table, ssize_t idx)
{
	return (TableViewCell *)_arrCells->getObjectAtIndex(idx);
}
