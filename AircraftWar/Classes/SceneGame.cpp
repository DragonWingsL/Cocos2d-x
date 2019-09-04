#include "SceneGame.h"
#include "ScrollBackground.h"
#include "Hero.h"
#include "AI.h"
#include "SceneMenu.h"

bool SceneGame::init(int planeID)
{
	Layer::init();
	setPosition(originPos);

	// 滚动背景
	ScrollBackground* bg = ScrollBackground::create();
	addChild(bg);

	// 右下角的返回按钮
	MenuItem* item = Util::addMenuItemImageRB(IMAGE_btn_back_normal, IMAGE_btn_back_press, this);
	item->setTarget(this, menu_selector(SceneGame::Back));

	// 创建英雄飞机
	addHeroPlane(planeID);
	
	// 使能touch
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	
	// 创建敌机AI
	createEnemyAI();
	
	// 碰撞检测
	scheduleUpdate();
	
	// 增加Label显示信息
	addLabels();
	
	return true;
}

void SceneGame::addLabels()
{
	CCLOG("hero hp: %d", _hero->_hp);
	_hp = LabelAtlas::create(Util::format(_hero->_hp), IMAGE_FONT_labelatlasimg, 24, 32, '0');
	_score = LabelAtlas::create("0", IMAGE_FONT_labelatlasimg, 24, 32, '0');

	addChild(_hp);
	addChild(_score);

	_hp->setAnchorPoint(Vec2(1, 1));
	_hp->setPosition(Vec2(winSize.width, winSize.height));

	_score->setAnchorPoint(Vec2(0, 1));
	_score->setPosition(Vec2(0, winSize.height));
}

void SceneGame::update(float)
{
	// 碰撞检测
	// 敌机的循环
	__Array *removeEnemies = __Array::create();
	removeEnemies->retain();

	int count = _ai->_enemys->count();
	for (int i = count - 1; i >= 0; --i)
	{
		Enemy* e = (Enemy*)_ai->_enemys->getObjectAtIndex(i);
		// 敌机和英雄战机是否有交集
		if (e->getBoundingBox().intersectsRect(_hero->getBoundingBox()))
		{
			// 英雄战机少血
			_hero->_hp -= e->_hp;
			
			if (_hero->_hp <= 0)
			{
				_hero->killed();

				// 切换到其他场景
				switchScene(SceneMenu);
				return;
			}

			// 注意避免显示负数的_hp
			Util::modifyLabel(_hp, -e->_hp);

			// 敌机从数组里删除，从数组删除意味着这个敌机不再参与碰撞检测
			// 也不再发射子弹
			// _ai->_enemys->fastRemoveObject(i);

			// 敌机死亡的动画，以及敌机从渲染树中删除
			// e->killed();
			removeEnemies->addObject(e);
			Util::modifyLabel(_score, e->_score);

			// 调用continue
			continue;
		}

#if 1
		__Array *removeHeroBullets = __Array::create();
		removeHeroBullets->retain();

		// 敌机和子弹做碰撞检测
		int bulletCount = _hero->_bullets->count();
		for (int j = bulletCount - 1; j >= 0; --j)
		{
			Sprite* sprite = (Sprite*)_hero->_bullets->getObjectAtIndex(j);
			if (sprite->getBoundingBox().intersectsRect(e->getBoundingBox()))
			{
				// 敌机的血要减少
				e->_hp -= _hero->_damage;

				// 判断敌机是否已经死亡
				if (e->_hp <= 0)
				{
					// 敌机从数组里删除，从数组删除意味着这个敌机不再参与碰撞检测
					Util::modifyLabel(_score, e->_score);
					// _ai->_enemys->removeObjectAtIndex(i);
					// e->killed();
					removeEnemies->addObject(e);
				}

				// 子弹要消失
				// sprite->removeFromParent();
				// _hero->_bullets->removeObject(sprite);
				removeHeroBullets->addObject(sprite);
				break;
			}
		}
		for (const auto &o : *removeHeroBullets) {
			Sprite *b = (Sprite *)o;
			b->removeFromParent();
			_hero->_bullets->removeObject(b);
		}
		removeHeroBullets->removeAllObjects();
		removeHeroBullets->release();
#endif 
	}

	for (const auto &o : *removeEnemies) {
		Enemy *e = (Enemy *)o;
		_ai->_enemys->removeObject(e);
		e->killed();
	}

	removeEnemies->removeAllObjects();
	removeEnemies->release();

	__Array *removeEnemyBullets = __Array::create();
	removeEnemyBullets->retain();

	// 敌机子弹的碰撞检测
	count = _ai->_bullets->count();
	for (int i = count - 1; i >= 0; --i)
	{
		Bullet* eb = (Bullet*)_ai->_bullets->getObjectAtIndex(i);
		if (eb->getBoundingBox().intersectsRect(_hero->getBoundingBox()))
		{
			_hero->_hp -= eb->_damage;

			if (_hero->_hp <= 0)
			{
				_hero->killed();
				switchScene(SceneMenu);
				return;
			}
			Util::modifyLabel(_hp, -eb->_damage);

			// eb->removeFromParent();
			// _ai->_bullets->removeObject(eb);

			removeEnemyBullets->addObject(eb);
			continue;  // 注意检测完成就不需要执行后续的代码了
		}

#if 1
		__Array *removeHeroBullets = __Array::create();
		removeHeroBullets->retain();

		// 敌机子弹和子弹做碰撞检测
		int bulletCount = _hero->_bullets->count();
		for (int j = bulletCount - 1; j >= 0; --j)
		{
			Sprite* sprite = (Sprite*)_hero->_bullets->getObjectAtIndex(j);
			Rect rc1 = sprite->getBoundingBox();
			Rect rc2 = eb->getBoundingBox();
			if (rc1.intersectsRect(rc2))
			{
				// 敌机子弹消失
				// eb->removeFromParent();
				// _ai->_bullets->removeObject(eb);
				removeEnemyBullets->addObject(eb);

				// 我方子弹要消失
				// sprite->removeFromParent();
				// _hero->_bullets->removeObject(sprite);
				removeHeroBullets->addObject(sprite);
				break;
			}
		}
		for (const auto &o : *removeHeroBullets) {
			Sprite *b = (Sprite *)o;
			b->removeFromParent();
			_hero->_bullets->removeObject(b);
		}
		removeHeroBullets->removeAllObjects();
		removeHeroBullets->release();
#endif
	}

	for (const auto &o : *removeEnemyBullets) {
		Bullet *eb = (Bullet *)o;
		eb->removeFromParent();
		_ai->_bullets->removeObject(eb);
	}
	removeEnemyBullets->removeAllObjects();
	removeEnemyBullets->release();

}

void SceneGame::createEnemyAI()
{
	_ai = AI::create();
	addChild(_ai);
}

void SceneGame::addHeroPlane(int id)
{
	_hero = Hero::create(id);
	addChild(_hero);
	_hero->setPosition(Vec2(winSize.width * ONE_OVER_TWO, _hero->getContentSize().height));
}

void SceneGame::Back(Ref*)
{
	switchScene(SceneMenu);
}

void SceneGame::onTouchMoved(Touch *t, Event *)
{
	if (Util::isPartOutOfWindow(_hero, t->getDelta()))
		return;

	Util::moveNode(_hero, t->getDelta());
}