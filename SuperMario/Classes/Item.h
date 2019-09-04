
#ifndef	__Item_H__
#define	__Item_H__

#include "Common.h"
#include "Mario.h"
class Mario;

class Item : public Sprite
{
public:
	static Item* create(ValueMap dict);

	virtual void move(float dt) = 0; 
	virtual void collision(Mario* mario) = 0;

	TMXTiledMap* getMap(){return (TMXTiledMap*)getParent();}

	Item_Type _type;
	bool _bDead;
};

#endif
