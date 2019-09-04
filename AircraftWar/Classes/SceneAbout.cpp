#include "SceneAbout.h"
#include "SceneStart.h"

bool SceneAbout::init()
{
	Layer::init();
	setPosition(originPos);

	// ����һ������ͼƬ
	Util::addBackground(IMAGE_background, this);

	// ����һ����ť�����½�
	MenuItem* item = Util::addMenuItemImageRB(IMAGE_btn_back_normal, IMAGE_btn_back_press, this);
	item->setTarget(this, menu_selector(SceneAbout::Back));

	// ���About��Ϣ
	setInfo();

	return true;
}

void SceneAbout::setInfo()
{
	__Dictionary* dict = __Dictionary::createWithContentsOfFile(CFG_about);
	Vec2 margin(0, 0);
	float space = 80;
	for (int i = 0; i < 4; ++i)
	{
		const __String* value = dict->valueForKey(Util::format(i + 1, "info"));
		LabelTTF* label = LabelTTF::create(value->getCString(), "Arial", 25);
		addChild(label);
		// label��ê�㣬����Ӱ��setPosition����ָ�������ֵĶ��뷽ʽ
		label->setAnchorPoint(Vec2(0, 1));
		label->setPosition(Vec2(margin.x, winSize.height - i * space - margin.y));

		label->setColor(Color3B(255, 255, 0));

		CCLOG("info:%s", value->getCString());
	}
}

void SceneAbout::Back(Ref*)
{
	switchScene(SceneStart);
}