
#include "ScrollBackground.h"

bool ScrollBackground::init()
{
	Layer::init();
	setPosition(originPos);

	Sprite* bg = Util::addBackground(IMAGE_background, this);

	Size backSize(winSize.width, winSize.height * 2);
	bg->setContentSize(backSize);

	// 让背景图片和窗口下边对其
	bg->setPositionY(bg->getPositionY() + backSize.height * .25f);

	MoveBy* move = MoveBy::create(5.0f, Vec2(0, -backSize.height * ONE_OVER_TWO));
	Place* place = Place::create(bg->getPosition());
	
	bg->runAction(RepeatForever::create(Sequence::create(move, place, NULL)));
	
	return true;
}
