
#include "ItemFireString.h"
#include "Mario.h"

void ItemFireString::move(float dt)
{
	if(_status == STOP)
	{
		_status = MOVE;
		RotateBy* rotate = RotateBy::create(_time, 270);
		runAction(RepeatForever::create(rotate));
	}
}

void ItemFireString::collision(Mario* mario)
{
	//两线之间的碰撞检测
	struct Line 
	{
		Vec2 pt1;
		Vec2 pt2;
	} marioLine[4];

	Line fireLine;

	Rect rc = mario->getBoundingBox();
	marioLine[0].pt1 = Vec2(rc.getMinX(), rc.getMinY());
	marioLine[0].pt2 = Vec2(rc.getMinX(), rc.getMaxY());

	marioLine[1].pt1 = Vec2(rc.getMaxX(), rc.getMinY());
	marioLine[1].pt2 = Vec2(rc.getMaxX(), rc.getMaxY());

	marioLine[2].pt1 = Vec2(rc.getMinX(), rc.getMinY());
	marioLine[2].pt2 = Vec2(rc.getMaxX(), rc.getMinY());

	marioLine[3].pt1 = Vec2(rc.getMinX(), rc.getMaxY());
	marioLine[3].pt2 = Vec2(rc.getMaxX(), rc.getMaxY());

	fireLine.pt1 = getPosition();
	int angle = getRotation();
	rc = getBoundingBox();
	angle %= 360;

	if(angle >270)
		fireLine.pt2 = Vec2(fireLine.pt1.x + rc.size.width, fireLine.pt1.y + rc.size.height);
	else if (angle > 180)
		fireLine.pt2 = Vec2(fireLine.pt1.x - rc.size.width, fireLine.pt1.y + rc.size.height);
	else if(angle > 90)
		fireLine.pt2 = Vec2(fireLine.pt1.x - rc.size.width, fireLine.pt1.y - rc.size.height);
	else 
		fireLine.pt2 = Vec2(fireLine.pt1.x + rc.size.width, fireLine.pt1.y - rc.size.height);

	for (int i = 0; i < 4; i++)
	{
		if(Vec2::isSegmentIntersect(marioLine[i].pt1, marioLine[i].pt2, fireLine.pt1, fireLine.pt2))
		{
			
			mario->killed(false);
		}
	}

}