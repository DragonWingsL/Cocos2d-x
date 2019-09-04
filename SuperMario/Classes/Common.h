
#ifndef	__Common_H__
#define	__Common_H__

#include "Ulit.h"
#include "Macro.h"

class Common
{
public:
	enum DIR{DIR_UP, DIR_RIGHT, DIR_DONW, DIR_LEFT, DIR_NONE};

	static bool canMoveLeft(Node* node, TMXTiledMap* map, float off)
	{
		//�ж������� �ƶ����Ƿ������ϰ���
		Rect rc = node->getBoundingBox();
		//��ȡ�ƶ����λ��
		rc.origin.x -= off;
		//����ұ�������
		Vec2 pt[3];
		pt[0] = Vec2(rc.getMinX(), rc.getMidY());
		pt[1] = Vec2(rc.getMinX(), rc.getMinY());
		pt[2] = Vec2(rc.getMinX(), rc.getMaxY());

		//��ù����ƶ����ڵ�ͼ��λ��
		char* layer[] = {"block", "pipe", "land"};
		for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int gid = Util::getmapGID(map, layer[i], pt[j]);
				//������������� �����ƶ����λ�ò��ǿյľ������ϰ��� 0�����ǿ�
				if (gid != 0)
					return false;
			}
		}
		return true;
	}

	static bool canMoveRight(Node* node, TMXTiledMap* map, float off)
	{
		//�ж������� �ƶ����Ƿ������ϰ���
		Rect rc = node->getBoundingBox();
		//��ȡ�ƶ����λ��
		rc.origin.x +=off;
		//������������
		Vec2 pt[3];
		pt[0] = Vec2(rc.getMaxX(), rc.getMidY());
		pt[1] = Vec2(rc.getMaxX(), rc.getMinY());
		pt[2] = Vec2(rc.getMaxX(), rc.getMaxY());

		//��ù����ƶ����ڵ�ͼ��λ��
		char* layer[] = {"block", "pipe", "land"};
		for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int gid = Util::getmapGID(map, layer[i], pt[j]);
				//������������� �����ƶ����λ�ò��ǿյľ������ϰ��� 0�����ǿ�
				if (gid != 0)
					return false;
			}
		}
		return true;
	}

	static bool canMoveUp();
	static bool canMoveDown(Node* node, TMXTiledMap* map, float off)
	{
		//�ж������� �ƶ����Ƿ������ϰ���
		Rect rc = node->getBoundingBox();
		//��ȡ�ƶ����λ��
		rc.origin.y -=off;
		//�������������
		Vec2 pt[3];
		pt[0] = Vec2(rc.getMidX(), rc.getMinY());
		pt[1] = Vec2(rc.getMinX(), rc.getMinY());
		pt[2] = Vec2(rc.getMaxX(), rc.getMinY());

		//��ù����ƶ����ڵ�ͼ��λ��
		char* layer[] = {"block", "pipe", "land"};
		for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int gid = Util::getmapGID(map, layer[i], pt[j]);
				//������������� �����ƶ����λ�ò��ǿյľ������ϰ��� 0�����ǿ�
				if (gid != 0)
					return false;
			}
		}
		return true;
	}
};



#endif