
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

	void loadMap(); //���ص�ͼ
	void loadControls(); //��ӿ������
	void loadResource(); //���ظ�����Դ
	void initMario(); //��ʼ�������
	void loadAnimation(const char* picName, const char* frameName, Size picSize, float frameDetal, int frameCount,int frameIdx = 0); //������Դ��ͨ�ú���
	void loadFrame(const char* picName, const char* frameName, Size frameSize, int idx = 0);		//���ص�֡��ͼƬ
	void initItem();

	//Mario �ƶ��ӿں���
	virtual void runLeft();
	virtual void runRight();
	virtual void stop();
	virtual void Jump();
	virtual void Fire();

	void checkFlag(); //�������������ӵ���ײ
	void checkMarioDead(); //���������Ƿ��ѽ���������������

	void update(float delta);
	void victory(); //ʤ��ͨ��
	void destoryBridge();
	void fireBullet(ItemBoss*);
	void checkCoin(); //���������Ƿ�Ե���Ӳ��
	void gameMenu(Ref*); //��Ϸ�˵� ��ͣ�ȵķ���

public:
	CC_SYNTHESIZE(int , _index, Index); //level
	CC_SYNTHESIZE(TMXTiledMap*, _map, TileMap);//��ͼ
	CC_SYNTHESIZE(Mario*, _Mario, Mario); //�����
	
	LayerDirCtrl* _ctrl; //�����
	Common::DIR _dir; //�������ǰ�ķ���
	__Array* _item;
	bool _bBig; //������ ͨ��ʱ��Ҫ��
	Vec2 _bridgePoint;

};
#endif