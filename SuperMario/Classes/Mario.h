
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

	//����Mario��״̬
	CC_PROPERTY(Common::DIR, _dir, Dir); //�˶�����
	CC_SYNTHESIZE(Common::DIR, _face, Face); //����λ��
	CC_SYNTHESIZE(bool, _big, Big); //���
	CC_SYNTHESIZE(bool, _air, Air); //�Ƿ��ڿ���
	CC_SYNTHESIZE(bool, _shoot, Shoot); //

	enum STATUS{DEAD_NONE, DEAD_FALL, DEAD_COLLISTION}; //û���� ��������գ� ײ����
	CC_SYNTHESIZE(STATUS, _dead, Dead); //�Ƿ�����
	CC_SYNTHESIZE(int, _speed, Speed); //ˮƽ�����ٶ�
	CC_SYNTHESIZE(int, _speedUp, SpeedUp); //��Ծ���ٶ�
	CC_SYNTHESIZE(int, _speedDown, SpeedDown); //�½����ٶ�
	CC_SYNTHESIZE(int, _gravity, Gravity); //Mario������ 
	CC_SYNTHESIZE(bool, _bAutoCtrl, AutoCtrl); //�������Ӻ����ƶ�
	CC_SYNTHESIZE(bool, _bAutoRun, AutoRun);//�Զ���
	CC_PROPERTY(bool, _touchFlag, TouchFlag); //�������ӵı��

	bool _bProtected; //����״̬
	bool _onLadder;
	Vec2 _ptInFinal;
	bool _bJump; //�յ��ʱ����
	int _speedV;


	void autoRun()
	{
			if(_bAutoRun)
				return;
			this->setDir(Common::DIR_RIGHT);
			this->updateMario();
			this->setSpeed(40);
			_bAutoRun = true;
			//�ж��յ��������滹��������
			if (_ptInFinal.y > 100)
			{
				//�յ���¥��
				_bJump = true;
			}
			scheduleUpdate();
	}

	//����Mario�Ķ���
	void updateMario();
	void updateMarioFire(); //��shoot �ӳ� 
	void updateMarioNoFire();
	void jump();

	void killed(bool bDead);
	void eatItem(Item_Type type);

	void upBlock(const char* layerName, Vec2 pt); //�������� ����action
	void update(float delta);
	void Fire();
	void setProtectedFalse(float)
	{
		_bProtected = false;
		setOpacity(225); //��͸�������û᲻͸��
	}
};
#endif