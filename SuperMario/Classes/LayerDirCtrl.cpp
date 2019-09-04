
#include "LayerDirCtrl.h"

void LayerDirCtrl::onEnter()
{
	Layer::onEnter();

	//因为方向键的精灵要经常换 所以先加载到内存
	
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(IMAGE_CONTROLS_UI_NONE);
	_frameNone = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	_frameNone->retain(); //不 retain 下一帧循环就会被删除
	
	texture = Director::getInstance()->getTextureCache()->addImage(IMAGE_CONTROLS_UI_LEFT);
	_frameLeft = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	_frameLeft->retain();

	texture = Director::getInstance()->getTextureCache()->addImage(IMAGE_CONTROLS_UI_RIGHT);
	_frameRight = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	_frameRight->retain();

	//添加跳跃和攻击键
	texture = Director::getInstance()->getTextureCache()->addImage(IMAGE_CONTROLS_UI_JUMP_NORMAL);
	_frameCtrlNormal = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	_frameCtrlNormal->retain();

	texture = Director::getInstance()->getTextureCache()->addImage(IMAGE_CONTROLS_UI_JUMP_SELECT);
	_frameCtrlSelect = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	_frameCtrlSelect->retain();

	_jump = Sprite::create();
	addChild(_jump);

	_fire = Sprite::create();
	addChild(_fire);

	//一开始都没有被按下所以是normal 模式
	_jump->setSpriteFrame(_frameCtrlNormal);
	_fire->setSpriteFrame(_frameCtrlNormal);

	_jump->setScale(1.8f);
	_fire->setScale(1.8f);

	_jump->setPosition(Vec2(winSize.width * .735f, winSize.height * .14f));
	_fire->setPosition(Vec2(winSize.width * .9f, winSize.height * .14f));


	//一开始是none 所以精灵是none
	_sprite = Sprite::create();
	_sprite->setSpriteFrame(_frameNone);
	addChild(_sprite);
	_sprite->setScale(2.f);
	_sprite->setPosition(Vec2(getContentSize().width * .15f, getContentSize().height * .2f));

	_jumpOrFireID = -1;
	_CtrlID = -1;

	//处理触摸事件
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	//一开始设置代理为空
	setDelegate(NULL);
}

bool LayerDirCtrl::jumpOrFire(Touch* pTouch)
{
	Rect rc = _jump->getBoundingBox();

	if (rc.containsPoint(pTouch->getLocation()))
	{
		_jump->setSpriteFrame(_frameCtrlSelect);
		if (_dirDelegate) _dirDelegate->Jump();
		_jumpOrFireID = pTouch->getID();
		return true;
	}

	rc = _fire->getBoundingBox();
	if (rc.containsPoint(pTouch->getLocation()))
	{
		_fire->setSpriteFrame(_frameCtrlSelect);
		if (_dirDelegate) _dirDelegate->Fire();
		_jumpOrFireID = pTouch->getID();
		return true;
	}

	return false;
}

bool LayerDirCtrl::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	bool b = jumpOrFire(pTouch);
	if (!b)
	{
		handleTouch(pTouch);
		if(_CtrlID == -1)
		_CtrlID = pTouch->getID();
	}
	return true;
}

void LayerDirCtrl::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (_CtrlID != -1)
		handleTouch(pTouch);
}

void LayerDirCtrl::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (_jumpOrFireID != -1)
	{
		_jumpOrFireID = -1;
		_jump->setSpriteFrame(_frameCtrlNormal);
		_fire->setSpriteFrame(_frameCtrlNormal);
	}
	else if (_CtrlID != -1)
	{
		_CtrlID = -1;
		_sprite->setSpriteFrame(_frameNone);
		setCtrlDir(Common::DIR_NONE);
		if (_dirDelegate) _dirDelegate->stop();
	}
	


}

void LayerDirCtrl::handleTouch(Touch* pTouch)
{
	//判断点中的点是在左边还是右边
	//左边就换左边的图
	//右边就换右边的图
	Rect rcSprite = _sprite->getBoundingBox();
	Size realSize(rcSprite.getMaxX() - rcSprite.getMinX(), rcSprite.getMaxY() - rcSprite.getMinY());
	//分成左右两边
	Rect rcLeft(rcSprite.origin.x, rcSprite.origin.y, realSize.width / 2, realSize.height);
	Rect rcRight(rcSprite.origin.x + realSize.width/2, rcSprite.origin.y, realSize.width / 2, realSize.height);

	Vec2 ptInLayer = this->convertTouchToNodeSpace(pTouch);

	if (rcLeft.containsPoint(ptInLayer))
	{
		//如果每次都进行一次切换精灵帧 会浪费时间
	//	_sprite->setSpriteFrame(_frameLeft);
		setCtrlDir(Common::DIR_LEFT);
		if (_dirDelegate) _dirDelegate->runLeft();
	}
	else if (rcRight.containsPoint(ptInLayer))
	{
	//	_sprite->setSpriteFrame(_frameRight);
		setCtrlDir(Common::DIR_RIGHT);
		if (_dirDelegate) _dirDelegate->runRight();
	}
	else
	{
	//	_sprite->setSpriteFrame(_frameNone);
		setCtrlDir(Common::DIR_NONE);
		if (_dirDelegate) _dirDelegate->stop();
	}
}

void LayerDirCtrl::onExit()
{
	Layer::onExit();
	_frameNone->release();
	_frameLeft->release();
	_frameRight->release();
}

void LayerDirCtrl::setCtrlDir(Common::DIR dir)
{
	if (dir == _ctrlDir)
		return;
	else
	{
		_ctrlDir = dir;
	}

	if (_ctrlDir == Common::DIR_LEFT)
	{
		_sprite->setSpriteFrame(_frameLeft);
	}
	else if (_ctrlDir == Common::DIR_RIGHT)
	{
		_sprite->setSpriteFrame(_frameRight);
	}
	else
	{
		_sprite->setSpriteFrame(_frameNone);
	}

}

Common::DIR LayerDirCtrl::getCtrlDir() const
{
	return _ctrlDir;
}