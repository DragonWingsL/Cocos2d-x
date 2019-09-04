
#include "ItemFlyFish.h"
#include "Mario.h"

void ItemFlyFish::move(float dt)
{
#if 0
	if(_status == STOP)
		return;
#endif

	Vec2 ptInMap = getPosition();
	Vec2 ptInWorld = getMap()->convertToWorldSpace(ptInMap);
	if((ptInWorld.x - winSize.width) > 20)
	{
		return;
	}

	if(_status == STOP)
	{
		if(_offsetH > 0)
			_status = FLY_LEFT;
		else 
			_status = FLY_RIGHT;
		updateItem();
		raise();
	}

}

void ItemFlyFish::collision(Mario* mario)
{
	//���ڴ������棬���ý�����ײ���
	if(_status == STOP)
		return;

	//����������½�������ײ
	if(getBoundingBox().intersectsRect(mario->getBoundingBox()))
	{
		mario->killed(false);
	}
}

void ItemFlyFish::raise()
{
	if(_status == FLY_LEFT) _status = FLY_RIGHT;
	else _status = FLY_LEFT;
	updateItem();

	ccBezierConfig cfg;
	cfg.controlPoint_1 = Vec2(CCRANDOM_0_1()*_offsetH, CCRANDOM_0_1()*_offsetV);
	cfg.controlPoint_2 = Vec2(CCRANDOM_0_1()*_offsetH, CCRANDOM_0_1()*_offsetV);
	cfg.endPosition = Vec2(_offsetH, _offsetV);

	BezierBy* bezie = BezierBy::create(_duration, cfg);
	CallFunc* call = CallFunc::create([=]() {this->full(); });
	runAction(CCSequence::createWithTwoActions(bezie, call));

}

void ItemFlyFish::full()
{
	if(_status == FLY_LEFT) _status = FLY_RIGHT;
	else _status = FLY_LEFT;
	updateItem();

	ccBezierConfig cfg;
	cfg.controlPoint_1 = Vec2(-CCRANDOM_0_1()*_offsetH, -CCRANDOM_0_1()*_offsetV);
	cfg.controlPoint_2 = Vec2(-CCRANDOM_0_1()*_offsetH, -CCRANDOM_0_1()*_offsetV);
	cfg.endPosition = Vec2(-_offsetH, -_offsetV);

	BezierBy* bezie = BezierBy::create(_duration, cfg);
	CallFunc* call = CallFunc::create([=]() {this->raise(); });
	runAction(CCSequence::createWithTwoActions(bezie, call));
	
}
