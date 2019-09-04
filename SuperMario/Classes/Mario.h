
#ifndef	__Mario_H__
#define __Mario_H__

#include "Common.h"

class Mario : public Sprite
{
public:
	CREATE_FUNC(Mario);
	bool init();

	void runLeft(float dt);
	void runRight(float dt);
	void stop(float dt);
	void runUp(float dt);
	void runDown(float dt);
	bool canMoveRight(float dt);
	bool canMoveLeft(float dt);
	bool canMoveUp(float dt);
	bool canMoveDown(float dt);

	TMXTiledMap* getMap(){return (TMXTiledMap*)getParent();}

	//定义Mario的状态
	CC_PROPERTY(Common::DIR, _dir, Dir); //运动方向
	CC_SYNTHESIZE(Common::DIR, _face, Face); //脸的位置
	CC_SYNTHESIZE(bool, _big, Big); //变大
	CC_SYNTHESIZE(bool, _air, Air); //是否在空中
	CC_SYNTHESIZE(bool, _shoot, Shoot); //

	enum STATUS{DEAD_NONE, DEAD_FALL, DEAD_COLLISTION}; //没死， 掉落在虚空， 撞死的
	CC_SYNTHESIZE(STATUS, _dead, Dead); //是否死亡
	CC_SYNTHESIZE(int, _speed, Speed); //水平方向速度
	CC_SYNTHESIZE(int, _speedUp, SpeedUp); //跳跃的速度
	CC_SYNTHESIZE(int, _speedDown, SpeedDown); //下降的速度
	CC_SYNTHESIZE(int, _gravity, Gravity); //Mario的重力 
	CC_SYNTHESIZE(bool, _bAutoCtrl, AutoCtrl); //到达旗子后不能移动
	CC_SYNTHESIZE(bool, _bAutoRun, AutoRun);//自动走
	CC_PROPERTY(bool, _touchFlag, TouchFlag); //碰到旗子的标记

	bool _bProtected; //保护状态
	bool _onLadder;
	Vec2 _ptInFinal;
	bool _bJump; //终点的时候用
	int _speedV;


	void autoRun()
	{
			if(_bAutoRun)
				return;
			this->setDir(Common::DIR_RIGHT);
			this->updateMario();
			this->setSpeed(40);
			_bAutoRun = true;
			//判断终点是在上面还是在下面
			if (_ptInFinal.y > 100)
			{
				//终点在楼上
				_bJump = true;
			}
			scheduleUpdate();
	}

	//更新Mario的动作
	void updateMario();
	void updateMarioFire(); //有shoot 加成 
	void updateMarioNoFire();
	void jump();

	void killed(bool bDead);
	void eatItem(Item_Type type);

	void upBlock(const char* layerName, Vec2 pt); //顶到物体 进行action
	void update(float delta);
	void Fire();
	void setProtectedFalse(float)
	{
		_bProtected = false;
		setOpacity(225); //把透明度设置会不透明
	}
};
#endif