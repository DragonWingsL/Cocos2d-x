
#include "LayerAbout.h"
#include "LayerMenu.h"

bool LayerAbout::init()
{
	Layer::init();

	setPosition(originPos);

	Util::addBackground("bg.png", this);

	Dictionary* dict = Dictionary::createWithContentsOfFile("about.xml");

	const __String *str = dict->valueForKey("people1");

	_ttf = CCLabelTTF::create(str->getCString(), "Arial", 30);
	addChild(_ttf);
	//设置锚点在左上角
	_ttf->setAnchorPoint(Vec2(0, 1));
	_ttf->setPosition(Vec2(30, winSize.height-50));
	_ttf->setLocalZOrder(1000);
	_ttf->setColor(ccc3(0, 255, 0));

	MenuItem* item = Util::createButtonBk("backA.png", "backB.png", this);
	item->setScale(2.0f);
	Size realSize = Size(item->getContentSize()*item->getScale());
	item->setPosition(Vec2(winSize.width / 2 - realSize.width / 2, -winSize.height / 2 + realSize.height / 2));
	item->setTarget(this, menu_selector(LayerAbout::Back));

	return true;
}

void LayerAbout::Back(Ref* )
{
	switchScene(LayerMenu);
}