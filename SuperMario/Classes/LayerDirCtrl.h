
#ifndef	__LayerDirCtrl_H__
#define	__LayerDirCtrl_H__

#include "Common.h"

//创建一个方向键代理 谁继承就是谁负责处理 
class DirCtrlDelegate
{
public:
	//马里奥接口函数
	virtual void runLeft() = 0;
	virtual void runRight() = 0;
	virtual void stop() = 0;

	//攻击和跳跃
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
	bool jumpOrFire(Touch* pTouch); //点击了攻击还是跳跃键

public:
	//精灵的图片
	Sprite* _sprite;
	//方向键帧缓存
	CC_SYNTHESIZE(SpriteFrame*, _frameNone, FrameNone);
	CC_SYNTHESIZE(SpriteFrame*, _frameRight, FrameRight);
	CC_SYNTHESIZE(SpriteFrame*, _frameLeft, FrameLeft);
	CC_SYNTHESIZE(DirCtrlDelegate*, _dirDelegate, Delegate);
	CC_PROPERTY(Common::DIR, _ctrlDir, CtrlDir);
	SpriteFrame* _frameCtrlNormal;
	SpriteFrame* _frameCtrlSelect;

	Sprite* _jump; //跳跃键
	Sprite* _fire;  //攻击键

	int _jumpOrFireID;
	int _CtrlID;

};

#endif