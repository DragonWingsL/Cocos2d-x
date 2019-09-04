
#include "ItemTortoiseFly.h"
#include "Mario.h"

void ItemTortoiseFly::move(float dt)
{
	if(_dir == Common::DIR_UP)
	{
		setPositionY(getPositionY() + dt*_speed);
		if(getPositionY() > _top)
			_dir = Common::DIR_DONW;
	}
	else 
	{
		setPositionY(getPositionY() - dt*_speed);
		if(getPositionY() < _down)
			_dir = Common::DIR_UP;
	}
}

void ItemTortoiseFly::collision(Mario* mario)
{
	if (mario->getBoundingBox().intersectsRect(this->getBoundingBox()))
	{
		if ((mario->getBoundingBox().getMinY() - getBoundingBox().getMinY()) < (getBoundingBox().size.height / 3))
		{
			mario->killed(false);
		}
		else
		{
			mario->setSpeedUp(mario->getSpeedDown());
			mario->setSpeedDown(0);
		}
	}

}