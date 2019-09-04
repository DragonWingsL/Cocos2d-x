
#ifndef	__LayerDirCtrl_H__
#define	__LayerDirCtrl_H__

#include "Common.h"

//����һ����������� ˭�̳о���˭������ 
class DirCtrlDelegate
{
public:
	//����½ӿں���
	virtual void runLeft() = 0;
	virtual void runRight() = 0;
	virtual void stop() = 0;

	//��������Ծ
	virtual void Jump() = 0;
	virtual void Fire() = 0;

};

class LayerDirCtrl : public Layer
{
public:
	CREATE_FUNC(LayerDirCtrl);
	
	void onEnter();
	void onExit();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	void handleTouch(Touch* pTouch);
	bool jumpOrFire(Touch* pTouch); //����˹���������Ծ��

public:
	//�����ͼƬ
	Sprite* _sprite;
	//�����֡����
	CC_SYNTHESIZE(SpriteFrame*, _frameNone, FrameNone);
	CC_SYNTHESIZE(SpriteFrame*, _frameRight, FrameRight);
	CC_SYNTHESIZE(SpriteFrame*, _frameLeft, FrameLeft);
	CC_SYNTHESIZE(DirCtrlDelegate*, _dirDelegate, Delegate);
	CC_PROPERTY(Common::DIR, _ctrlDir, CtrlDir);
	SpriteFrame* _frameCtrlNormal;
	SpriteFrame* _frameCtrlSelect;

	Sprite* _jump; //��Ծ��
	Sprite* _fire;  //������

	int _jumpOrFireID;
	int _CtrlID;

};

#endif