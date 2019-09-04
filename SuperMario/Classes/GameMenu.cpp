
#include "GameMenu.h"
#include "LayerGame.h"
#include "LayerStart.h"

void GameMenu::onEnter()
{
	Layer::onEnter();

	//���ģ̬�Ի���ͼƬ
	Node *background = Util::addBackground("Set_Menu.png", this);
	background->setContentSize(winSize * .5f);

	//���һ��ģ̬�Ի���
	MenuItemImage* reSume = MenuItemImage::create("resume_n.png", "resume_s.png", this, menu_selector(GameMenu::Continue));
	MenuItemImage* reStart = MenuItemImage::create("restart_n.png", "restart_s.png", this, menu_selector(GameMenu::Start));
	MenuItemImage* retry = MenuItemImage::create("select_n.png", "select_s.png", this, menu_selector(GameMenu::Retry));

	Menu* menu = Menu::create(reSume, reStart, retry, NULL);
	addChild(menu);

	menu->alignItemsVertically();

	//����ģ̬�Ի���ĵ�������¼���������
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	//setTouchPriority(-130);

	_menu = menu;
}

void GameMenu::Continue(Ref*)
{
	//�Ƴ�ģ̬�Ի���
	removeFromParent();
}

void GameMenu::Start(Ref*)
{
	LayerGame* game = (LayerGame*)getParent();
	switchScene(LayerGame, game->getIndex());
}

void GameMenu::Retry(Ref*)
{
	switchScene(LayerStart);
}


void GameMenu::onExit()
{
	Layer::onExit();
	Director::getInstance()->resume();
}

bool GameMenu::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	_bTouch = _menu->onTouchBegan(pTouch, pEvent);

	return true;
}

void GameMenu::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	if(_bTouch) _menu->onTouchCancelled(pTouch, pEvent);
}

void GameMenu::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if(_bTouch) _menu->onTouchEnded(pTouch, pEvent);
}

void GameMenu::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	if(_bTouch) _menu->onTouchMoved(pTouch, pEvent);
}