
#include <cocos2d.h>

#include "Level/PlayerElement.h"
#include "Level/CrateElement.h"
#include "CommonMacro.h"
#include "GlobalManager.h"
#include "Level/Level.h"

USING_NS_CC;

Level::Level(LEVEL_MAP &map)
{
	_map = map;
}

Level::~Level()
{
}

Level *Level::create(LEVEL_MAP &map)
{
	Level *pRet = new(std::nothrow) Level(map);
	if (pRet && pRet->initWithMap(map))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Level::initWithMap(LEVEL_MAP &map)
{
	if (!Layer::init())
		return false;

	auto manager = GlobalManager::getInstance();

	_endpointCount = 0;
	_endpointTriggerCount = 0;

	Vec2 ltPos = manager->LevelLeftTopPos;
	int minX = INT_MAX, maxX = 0, playerX = 0;
	_horiCellCount = _map[0].size();
	for (int i = 0, ic = _map.size(); i < ic; ++i) {
		std::vector<int> row = _map[i];
		for (int j = 0, jc = row.size(); j < jc; ++j) {
			int flag = row[j];
			if (!IS_EMPTY(flag)) {
				int idx = i * _horiCellCount + j;
				minX = MIN(minX, j);
				maxX = MAX(maxX, j);

				Vec2 elementPos(j * manager->CellSize.width, -i * manager->CellSize.height);
				elementPos.add(ltPos);
				// Wall
				if (flag == 0) {
					auto e = spawn(ElementType::Wall, elementPos);
					addElement(idx, e);
				}
				// Crate
				else if (flag == 1) {
					auto e = spawn(ElementType::Floor, elementPos);
					addElement(idx, e);
					e = spawn(ElementType::Crate, elementPos);
					addElement(idx, e, 2);
				}
				// Endpoint
				else if (flag == 2) {
					auto e = spawn(ElementType::Floor, elementPos);
					addElement(idx, e);
					auto endpoint = spawn(ElementType::EndPoint, elementPos);
					endpoint->setScale(.5f);
					addElement(idx, endpoint, 1);
					++_endpointCount;
				}
				// Player
				else if (flag == 3) {
					auto e = spawn(ElementType::Floor, elementPos);
					addElement(idx, e);
					_player = (PlayerElement *)spawn(ElementType::Player, elementPos);
					addElement(idx, _player, 2);
					playerX = j;
				}
				// Floor
				else if (flag == 4) {
					auto e = spawn(ElementType::Floor, elementPos);
					addElement(idx, e);
				}
				// CrateTriggerOfEndpoint
				else if (flag == 10) {
					auto e = spawn(ElementType::Floor, elementPos);
					addElement(idx, e);
					e = spawn(ElementType::CrateTriggerOfEndPoint, elementPos);
					addElement(idx, e, 2);
					auto endpoint = spawn(ElementType::EndPoint, elementPos);
					endpoint->setScale(.5f);
					addElement(idx, endpoint, 1);
					++_endpointTriggerCount;
					++_endpointCount;
				}
			}
		}
	}
	setPosition(getPosition() - Vec2(minX * manager->CellSize.width, 0));
	_rightMark = _leftMark = _curMark = 0;
	
	int deltaX = manager->HorizontalCellCount - (maxX - minX) - 1;
	if (deltaX > 0) {
		Vec2 deltaPos;
		if (deltaX % 2 == 0) {
			deltaPos = Vec2(deltaX * ONE_OVER_TWO * manager->CellSize.width, 0);
		}
		else {
			deltaPos = Vec2(((int)(deltaX * ONE_OVER_TWO) + 1) * manager->CellSize.width, 0);
		}
		setPosition(getPosition() + deltaPos);
	}
	else {
		_rightMark += abs(deltaX);
	}

	Vec2 movePos = Vec2::ZERO;
	deltaX = playerX - minX + 1;
	if (deltaX >= 9) {
		int marginX =  maxX - playerX;
		int moveX = 0;
		if (marginX >= 3) {
			moveX = playerX + 3 - minX - manager->HorizontalCellCount + 1;
			_curMark = moveX;
		}
		else {
			moveX = maxX - minX - manager->HorizontalCellCount + 1;
			_curMark = _rightMark;
		}
		movePos.x = moveX * manager->CellSize.width;
	}
	setPosition(getPosition() - movePos);

	return true;
}

PlayerElement *Level::getPlayer() const
{
	return _player;
}

LevelElement *Level::getElementByPosition(Vec2 &pos) const
{
	int idx = convertPositionToLevelIdx(pos);
	auto target = _idxOfElements.find(idx);
	std::vector<LevelElement *> vector;
	if (target != _idxOfElements.end()) {
		vector = target->second;
	}
	else {
		return nullptr;
	}

	int maxZOrder = INT_MIN;
	for (const auto &e : vector) {
		maxZOrder = MAX(maxZOrder, e->getLocalZOrder());
	}

	LevelElement *element = nullptr;
	for (const auto &e : vector) {
		if (e->getLocalZOrder() == maxZOrder)
			element = e;
	}

	return element;
}

ssize_t Level::getHorizontalCellCount() const
{
	return _horiCellCount;
}

void Level::resetIdxOfElement(int idx, LevelElement *element)
{
	int originalIdx = element->getLevelIdx();
	auto target = _idxOfElements.find(originalIdx);
	if (target != _idxOfElements.end()) {
		std::vector<LevelElement *> &vector = target->second;
		int i = 0;
		for (const auto e : vector) {
			if (e == element) {
				vector.erase(vector.begin() + i);
				break;
			}
			++i;
		}
		addElement(idx, element, element->getLocalZOrder(), false);
	}
	else {
		return;
	}
}

int Level::convertPositionToLevelIdx(Vec2 &pos) const
{
	auto manager = GlobalManager::getInstance();
	Vec2 cvtToLevel = pos - manager->LevelLeftTopPos;
	Vec2 posToLevel = Vec2(
		abs(round(cvtToLevel.x / manager->CellSize.width)),
		abs(round(cvtToLevel.y / manager->CellSize.height))
	);

	return (int)posToLevel.y * _horiCellCount + (int)posToLevel.x;
}

ssize_t Level::getEndpointCount() const
{
	return _endpointCount;
}

ssize_t Level::getDefaultEndpointTriggerCount() const
{
	return _endpointTriggerCount;
}

void Level::move(bool isLeft)
{
	auto manager = GlobalManager::getInstance();
	Vec2 pos = Vec2::ZERO;
	if (isLeft) {
		if (_curMark > _leftMark) {
			pos.x += manager->CellSize.width;
			--_curMark;
		}
	}
	else {
		if (_curMark < _rightMark) {
			pos.x -= manager->CellSize.width;
			++_curMark;
		}
	}
	runAction(MoveBy::create(.5f, pos));
}

LevelElement *Level::spawn(ElementType type, cocos2d::Vec2 &pos)
{
	LevelElement *ele = nullptr;

	if (type == ElementType::Player) {
		ele = PlayerElement::create(type);
	}
	else if (type == ElementType::Crate) {
		ele = CrateElement::create(type);
	}
	else if (type == ElementType::CrateTriggerOfEndPoint) {
		ele = CrateElement::create(type);
		((CrateElement *)ele)->setStatus(CrateStatus::Trigger);
	}
	else {
		ele = LevelElement::create(type);
		ele->setContentSize(GlobalManager::getInstance()->CellSize);
	}
	ele->setPosition(pos);

	return ele;
}

void Level::addElement(int idx, LevelElement *element, int zOrder, bool isAdd)
{
	CCASSERT(!(element == nullptr), "Element can not be nullptr.");

	auto target = _idxOfElements.find(idx);

	if (target != _idxOfElements.end()) {
		target->second.push_back(element);
	}
	else {
		auto vector = std::vector<LevelElement *>();
		vector.push_back(element);
		_idxOfElements.insert(std::pair<int, std::vector<LevelElement *>>(idx, vector));
	}
	if (isAdd)
		addChild(element, zOrder);
	element->setLevelIdx(idx);
}
