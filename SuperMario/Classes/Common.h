
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
		//判断三个点 移动后是否碰到障碍物
		Rect rc = node->getBoundingBox();
		//获取移动后的位置
		rc.origin.x -= off;
		//获得右边三个点
		Vec2 pt[3];
		pt[0] = Vec2(rc.getMinX(), rc.getMidY());
		pt[1] = Vec2(rc.getMinX(), rc.getMinY());
		pt[2] = Vec2(rc.getMinX(), rc.getMaxY());

		//获得怪物移动后在地图的位置
		char* layer[] = {"block", "pipe", "land"};
		for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int gid = Util::getmapGID(map, layer[i], pt[j]);
				//如果这三个场景 怪物移动后的位置不是空的就是有障碍物 0代表是空
				if (gid != 0)
					return false;
			}
		}
		return true;
	}

	static bool canMoveRight(Node* node, TMXTiledMap* map, float off)
	{
		//判断三个点 移动后是否碰到障碍物
		Rect rc = node->getBoundingBox();
		//获取移动后的位置
		rc.origin.x +=off;
		//获得左边三个点
		Vec2 pt[3];
		pt[0] = Vec2(rc.getMaxX(), rc.getMidY());
		pt[1] = Vec2(rc.getMaxX(), rc.getMinY());
		pt[2] = Vec2(rc.getMaxX(), rc.getMaxY());

		//获得怪物移动后在地图的位置
		char* layer[] = {"block", "pipe", "land"};
		for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int gid = Util::getmapGID(map, layer[i], pt[j]);
				//如果这三个场景 怪物移动后的位置不是空的就是有障碍物 0代表是空
				if (gid != 0)
					return false;
			}
		}
		return true;
	}

	static bool canMoveUp();
	static bool canMoveDown(Node* node, TMXTiledMap* map, float off)
	{
		//判断三个点 移动后是否碰到障碍物
		Rect rc = node->getBoundingBox();
		//获取移动后的位置
		rc.origin.y -=off;
		//获得下面三个点
		Vec2 pt[3];
		pt[0] = Vec2(rc.getMidX(), rc.getMinY());
		pt[1] = Vec2(rc.getMinX(), rc.getMinY());
		pt[2] = Vec2(rc.getMaxX(), rc.getMinY());

		//获得怪物移动后在地图的位置
		char* layer[] = {"block", "pipe", "land"};
		for (int i = 0; i < sizeof(layer) / sizeof(*layer); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int gid = Util::getmapGID(map, layer[i], pt[j]);
				//如果这三个场景 怪物移动后的位置不是空的就是有障碍物 0代表是空
				if (gid != 0)
					return false;
			}
		}
		return true;
	}
};



#endif