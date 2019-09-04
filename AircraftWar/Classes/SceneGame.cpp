#include "SceneGame.h"
#include "ScrollBackground.h"
#include "Hero.h"
#include "AI.h"
#include "SceneMenu.h"

bool SceneGame::init(int planeID)
{
	Layer::init();
	setPosition(originPos);

	// ��������
	ScrollBackground* bg = ScrollBackground::create();
	addChild(bg);

	// ���½ǵķ��ذ�ť
	MenuItem* item = Util::addMenuItemImageRB(IMAGE_btn_back_normal, IMAGE_btn_back_press, this);
	item->setTarget(this, menu_selector(SceneGame::Back));

	// ����Ӣ�۷ɻ�
	addHeroPlane(planeID);
	
	// ʹ��touch
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	
	// �����л�AI
	createEnemyAI();
	
	// ��ײ���
	scheduleUpdate();
	
	// ����Label��ʾ��Ϣ
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
	// ��ײ���
	// �л���ѭ��
	__Array *removeEnemies = __Array::create();
	removeEnemies->retain();

	int count = _ai->_enemys->count();
	for (int i = count - 1; i >= 0; --i)
	{
		Enemy* e = (Enemy*)_ai->_enemys->getObjectAtIndex(i);
		// �л���Ӣ��ս���Ƿ��н���
		if (e->getBoundingBox().intersectsRect(_hero->getBoundingBox()))
		{
			// Ӣ��ս����Ѫ
			_hero->_hp -= e->_hp;
			
			if (_hero->_hp <= 0)
			{
				_hero->killed();

				// �л�����������
				switchScene(SceneMenu);
				return;
			}

			// ע�������ʾ������_hp
			Util::modifyLabel(_hp, -e->_hp);

			// �л���������ɾ����������ɾ����ζ������л����ٲ�����ײ���
			// Ҳ���ٷ����ӵ�
			// _ai->_enemys->fastRemoveObject(i);

			// �л������Ķ������Լ��л�����Ⱦ����ɾ��
			// e->killed();
			removeEnemies->addObject(e);
			Util::modifyLabel(_score, e->_score);

			// ����continue
			continue;
		}

#if 1
		__Array *removeHeroBullets = __Array::create();
		removeHeroBullets->retain();

		// �л����ӵ�����ײ���
		int bulletCount = _hero->_bullets->count();
		for (int j = bulletCount - 1; j >= 0; --j)
		{
			Sprite* sprite = (Sprite*)_hero->_bullets->getObjectAtIndex(j);
			if (sprite->getBoundingBox().intersectsRect(e->getBoundingBox()))
			{
				// �л���ѪҪ����
				e->_hp -= _hero->_damage;

				// �жϵл��Ƿ��Ѿ�����
				if (e->_hp <= 0)
				{
					// �л���������ɾ����������ɾ����ζ������л����ٲ�����ײ���
					Util::modifyLabel(_score, e->_score);
					// _ai->_enemys->removeObjectAtIndex(i);
					// e->killed();
					removeEnemies->addObject(e);
				}

				// �ӵ�Ҫ��ʧ
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

	// �л��ӵ�����ײ���
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
			continue;  // ע������ɾͲ���Ҫִ�к����Ĵ�����
		}

#if 1
		__Array *removeHeroBullets = __Array::create();
		removeHeroBullets->retain();

		// �л��ӵ����ӵ�����ײ���
		int bulletCount = _hero->_bullets->count();
		for (int j = bulletCount - 1; j >= 0; --j)
		{
			Sprite* sprite = (Sprite*)_hero->_bullets->getObjectAtIndex(j);
			Rect rc1 = sprite->getBoundingBox();
			Rect rc2 = eb->getBoundingBox();
			if (rc1.intersectsRect(rc2))
			{
				// �л��ӵ���ʧ
				// eb->removeFromParent();
				// _ai->_bullets->removeObject(eb);
				removeEnemyBullets->addObject(eb);

				// �ҷ��ӵ�Ҫ��ʧ
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