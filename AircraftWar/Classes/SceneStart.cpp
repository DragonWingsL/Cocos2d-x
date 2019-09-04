#include "SceneStart.h"
#include "SceneAbout.h"
#include "SceneMenu.h"

bool SceneStart::init()
{
	Layer::init();

	setPosition(originPos);

	// ����һ������ͼƬ
	Util::addBackground(IMAGE_background, this);

	// �������˵���ť
	MenuItem* start = MenuItemImage::create(IMAGE_start_1, IMAGE_start_2);
	MenuItem* about = MenuItemImage::create(IMAGE_about_1, IMAGE_about_2);
	MenuItem* quit  = MenuItemImage::create(IMAGE_quit_1, IMAGE_quit_2);
	Menu* menu = Menu::create(start, about, quit, NULL);
	addChild(menu);
	// �Զ���ո�Ķ���
	menu->alignItemsVerticallyWithPadding(50);

	// ������ť���ӻص�����
	start->setTarget(this, menu_selector(SceneStart::Start));
	about->setTarget(this, menu_selector(SceneStart::About));
	quit->setTarget(this, menu_selector(SceneStart::Quit));

	return true;
}

void SceneStart::Start(Ref*)
{
	switchScene(SceneMenu);
}

void SceneStart::About(Ref*)
{
	switchScene(SceneAbout);
//	Director::sharedDirector()->replaceScene(Util::scene(SceneAbout::create()));
}

void SceneStart::Quit(Ref*)
{
	Director::getInstance()->end();
}