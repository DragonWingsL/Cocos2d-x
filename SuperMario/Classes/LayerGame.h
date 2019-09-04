
#ifndef	__LayerGame_H__
#define	__LayerGame_H__

#include "Common.h"
#include "Mario.h"
#include "LayerDirCtrl.h"

class ItemBoss;

class LayerGame : public Layer, public DirCtrlDelegate
{
public:
	static LayerGame* create(int index, bool big = false)
	{
		LayerGame* game = new LayerGame;
		game->init();
		game->_bBig = big;
		game->setIndex(index);
		game->autorelease();
		game->setPosition(originPos);
		return game;
	}

	void onEnter();
	void onExit();

	void loadMap(); //加载地图
	void loadControls(); //添加控制面板
	void loadResource(); //加载各种资源
	void initMario(); //初始化马里奥
	void loadAnimation(const char* picName, const char* frameName, Size picSize, float frameDetal, int frameCount,int frameIdx = 0); //加载资源的通用函数
	void loadFrame(const char* picName, const char* frameName, Size frameSize, int idx = 0);		//加载单帧的图片
	void initItem();

	//Mario 移动接口函数
	virtual void runLeft();
	virtual void runRight();
	virtual void stop();
	virtual void Jump();
	virtual void Fire();

	void checkFlag(); //检测马里奥与旗子的碰撞
	void checkMarioDead(); //检测马里奥是否已进行死亡掉出界外

	void update(float delta);
	void victory(); //胜利通关
	void destoryBridge();
	void fireBullet(ItemBoss*);
	void checkCoin(); //检测马里奥是否吃到了硬币
	void gameMenu(Ref*); //游戏菜单 暂停等的服务

public:
	CC_SYNTHESIZE(int , _index, Index); //level
	CC_SYNTHESIZE(TMXTiledMap*, _map, TileMap);//地图
	CC_SYNTHESIZE(Mario*, _Mario, Mario); //马里奥
	
	LayerDirCtrl* _ctrl; //方向键
	Common::DIR _dir; //方向键当前的方向
	__Array* _item;
	bool _bBig; //保存变大 通关时候要用
	Vec2 _bridgePoint;

};
#endif