
#include "LayerMenu.h"
#include "LayerStart.h"
#include "LayerSetting.h"
#include "LayerAbout.h"

Scene* LayerMenu::scene()
{
	Scene* scene = Scene::create();
	LayerMenu* layer = LayerMenu::create();
	scene->addChild(layer);
	return scene;
}

bool LayerMenu::init()
{
	Layer::init();
	setPosition(originPos);

	//创建一个背景图片
	initBackground();
	//创建按钮
	initButton();

	return true;
}

void LayerMenu::initBackground()
{
	Util::addBackground(IMAGE_MENU_background, this);

}

void LayerMenu::initButton()
{
	MenuItem* itemStart = MenuItemImage::create(IMAGE_MENU_start_1, IMAGE_MENU_start_2, [this](Ref* r) {Start(r); });
	MenuItem* itemSetting = MenuItemImage::create(IMAGE_MENU_setting_1, IMAGE_MENU_setting_2, [this](Ref* r) {Setting(r); });
	MenuItem* itemQuit= MenuItemImage::create(IMAGE_MENU_quit_1, IMAGE_MENU_quit_2, [this](Ref* r) {Quit(r); });
	MenuItem* itemAbout = MenuItemImage::create(IMAGE_MENU_about_1, IMAGE_MENU_about_2, [this](Ref* r) {About(r); });

	itemStart->setScale(1.5f);
	itemSetting->setScale(1.5f);
	itemQuit->setScale(1.5f);
	itemAbout->setScale(1.5f);
	Menu* menu = Menu::create(itemStart, itemSetting, itemQuit, itemAbout, NULL);
	menu->setPosition(Vec2(0, -winSize.height * .05f) + winSize * .5f);
	addChild(menu);

	menu->alignItemsVertically();
	Vector<Node *> arrMenu = menu->getChildren();
	for(const auto &obj : arrMenu)
	{
		Node* node = (Node*)obj;
		Util::moveNode(node, Vec2(0, -70));
	}
}

void LayerMenu::Start(Ref*)
{
	switchScene(LayerStart);
}

void LayerMenu::Setting(Ref*)
{
	//TODO
	switchScene(LayerSetting);
}

void LayerMenu::Quit(Ref*)
{
	//退出游戏
	Director::getInstance()->end();
}

void LayerMenu::About(Ref*)
{
	//TODO
	switchScene(LayerAbout);
}