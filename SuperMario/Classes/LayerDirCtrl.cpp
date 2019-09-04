
#include "LayerDirCtrl.h"

void LayerDirCtrl::onEnter()
{
	Layer::onEnter();

	//��Ϊ������ľ���Ҫ������ �����ȼ��ص��ڴ�
	
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(IMAGE_CONTROLS_UI_NONE);
	_frameNone = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	_frameNone->retain(); //�� retain ��һ֡ѭ���ͻᱻɾ��
	
	texture = Director::getInstance()->getTextureCache()->addImage(IMAGE_CONTROLS_UI_LEFT);
	_frameLeft = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	_frameLeft->retain();

	texture = Director::getInstance()->getTextureCache()->addImage(IMAGE_CONTROLS_UI_RIGHT);
	_frameRight = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	_frameRight->retain();

	//�����Ծ�͹�����
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

	//һ��ʼ��û�б�����������normal ģʽ
	_jump->setSpriteFrame(_frameCtrlNormal);
	_fire->setSpriteFrame(_frameCtrlNormal);

	_jump->setScale(1.8f);
	_fire->setScale(1.8f);

	_jump->setPosition(Vec2(winSize.width * .735f, winSize.height * .14f));
	_fire->setPosition(Vec2(winSize.width * .9f, winSize.height * .14f));


	//һ��ʼ��none ���Ծ�����none
	_sprite = Sprite::create();
	_sprite->setSpriteFrame(_frameNone);
	addChild(_sprite);
	_sprite->setScale(2.f);
	_sprite->setPosition(Vec2(getContentSize().width * .15f, getContentSize().height * .2f));

	_jumpOrFireID = -1;
	_CtrlID = -1;

	//�������¼�
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	//һ��ʼ���ô���Ϊ��
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
	//�жϵ��еĵ�������߻����ұ�
	//��߾ͻ���ߵ�ͼ
	//�ұ߾ͻ��ұߵ�ͼ
	Rect rcSprite = _sprite->getBoundingBox();
	Size realSize(rcSprite.getMaxX() - rcSprite.getMinX(), rcSprite.getMaxY() - rcSprite.getMinY());
	//�ֳ���������
	Rect rcLeft(rcSprite.origin.x, rcSprite.origin.y, realSize.width / 2, realSize.height);
	Rect rcRight(rcSprite.origin.x + realSize.width/2, rcSprite.origin.y, realSize.width / 2, realSize.height);

	Vec2 ptInLayer = this->convertTouchToNodeSpace(pTouch);

	if (rcLeft.containsPoint(ptInLayer))
	{
		//���ÿ�ζ�����һ���л�����֡ ���˷�ʱ��
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