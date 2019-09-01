
#include <cocos2d.h>
#include <cocostudio/CocoStudio.h>

#include "User/UserData.h"
#include "Level/LevelData.h"
#include "Audio/AudioUtils.h"
#include "CommonMacro.h"
#include "GlobalManager.h"

USING_NS_CC;

GlobalManager *GlobalManager::_inst = new GlobalManager();

GlobalManager::GlobalManager() : 
	_levelDataFilePath("/Levels"),
	IsEnableSound(true), 
	IsEnableMusic(true),
	HorizontalCellCount(9),
	VerticalCellCount(13)
{
#if 1
	_spriteResourcesPathRecords.clear();

	_spriteResourcesPathRecords = { 

		/* *
		 * Font set image.
		 * */
		{h(AbcFontSetImage), "images/spritefont.png"},
		{h(NumFontSetImage), "images/spritefont2.png"},

		/* *
		 * MainMenuScene.
		 * */
		{h(MainMenuBackground), "images/bg_menu-sheet0.png"},
		{h(MainMenuTitle), "images/game_title-sheet0.png"},
		{h(MainMenuPlayBtn), "images/btn_play-sheet0.png"},
		{h(MainMenuMusicBtnNormal), "images/btn_music-sheet0.png"},
		{h(MainMenuMusicBtnClicked), "images/btn_music-sheet1.png"},
		{h(MainMenuSoundBtnNormal), "images/btn_sound-sheet0.png"},
		{h(MainMenuSoundBtnClicked), "images/btn_sound-sheet1.png"},
		{h(MainMenuInfoBtnNormal), "images/btn_info-sheet0.png"},
		{h(MainMenuInfo), "images/info.png"},
		{h(MainMenuInfoCloseBtnNormal), "images/btn_close-sheet0.png"},
		
		/* *
		 * LevelScene.
		 * */
		{h(LevelBackground), "images/bg_level-sheet0.png"},
		{h(LevelLockCrate), "images/btn_level-sheet0.png"},
		{h(LevelUnlockCrate), "images/btn_level-sheet1.png"},
		{h(LevelBackBtnNormal), "images/btn_back-sheet0.png"},
		{h(LevelPageBtnNormal), "images/page_dots-sheet0.png"},
		{h(LevelPageBtnSelected), "images/page_dots-sheet1.png"},
		
		/* *
		 * GameScene.
		 * */
		{h(GameBackgroundTile), "images/tiledbackground.png"},
		{h(GameMenuBackground), "images/footer-sheet0.png"},
		{h(GameMenuBackBtnSmall), "images/btn_to_level_window-sheet0.png"},
		{h(GameMenuBackBtnMiddle), "images/btn_to_level-sheet0.png"},
		{h(GameMenuRepentBtnNormal), "images/btn_undo-sheet0.png"},
		{h(GameMenuRestartBtnNormal), "images/btn_restart-sheet0.png"},
		{h(GameLevelWall), "images/wall-sheet0.png"},
		{h(GameLevelFloor), "images/floor1-sheet0.png"},
		{h(GameLevelEndPoint), "images/drop_point-sheet0.png"},
		{h(GameLevelCrateNormal), "images/crate-sheet0.png"},
		{h(GameLevelCrateTriggerOfEndPoint), "images/crate-sheet1.png"},
		{h(GamePlayerElementActionSetPart1), "images/player-sheet0.png"},
		{h(GamePlayerElementActionSetPart2), "images/player-sheet1.png"},
		{h(GameControl), "images/game_control-sheet0.png"},
		{h(GameOverlap), "images/overlay.png"},
		{h(GameNextLevelBtnNormal), "images/btn_next-sheet0.png"}
	};

	Size abcFontSize = Size(69, 97);
	int abcHoriCount = 14, abcTotalCount = 36;
	int i = 0;
	for (i = 0; i < 26; ++i) {
		_abcFontSetRectangles.insert(std::pair<char, Rect>(
			char(0x41 + i),
			Rect(
				Vec2(i % abcHoriCount * abcFontSize.width, 
					i / abcHoriCount * abcFontSize.height), 
				abcFontSize
			))
		);
	}

	for (; i < abcTotalCount; ++i) {
		_abcFontSetRectangles.insert(std::pair<char, Rect>(
			char(0x16 + i),
			Rect(
				Vec2(i % abcHoriCount * abcFontSize.width,
					i / abcHoriCount * abcFontSize.height),
				abcFontSize
			))
		);
	}

#endif 
}

GlobalManager::~GlobalManager()
{
	delete _userData;
	delete _levelData;
}

GlobalManager *GlobalManager::getInstance()
{
	return _inst;
}

UserData *GlobalManager::getUserData() const
{
	return _userData;
}

LevelData *GlobalManager::getLevelData() const
{
	return _levelData;
}

void GlobalManager::init()
{
	_ccDirector = Director::getInstance();
	_ccFileUtils = FileUtils::getInstance();
	_userData = new UserData();  UserData::setDelegate(_userData);
	_levelData = new LevelData();  LevelData::setDelegate(_levelData);
	AudioUtils::getInstance()->init();
	VisibleOriginPos = _ccDirector->getVisibleOrigin();
	VisibleSize = _ccDirector->getVisibleSize();
	VisibleCenterPos = VisibleOriginPos + VisibleSize * ONE_OVER_TWO;
	float sx = VisibleSize.width / (HorizontalCellCount * 1.0f);
	CellSize = Size(sx, sx);
	LevelLeftTopPos = VisibleOriginPos + Vec2(0, CellSize.height * VerticalCellCount) + 
		CellSize * ONE_OVER_TWO;

	if (!_levelData || 
		!_userData || 
		!_userData->saveDefaultUserData()) {
		ExitGame();
	}
	
}

void GlobalManager::onLoadCompletedCallback()
{
	auto manager = GlobalManager::getInstance();
	Size playerFrameSize = Size(72, 89);
	auto director = Director::getInstance();
	Texture2D *texPlayerActionSet1 = director->getTextureCache()->getTextureForKey(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GamePlayerElementActionSetPart1));
	Texture2D *texPlayerActionSet2 = director->getTextureCache()->getTextureForKey(manager->getSpriteResourcesPathForName(GlobalManager::EnumSpriteName::GamePlayerElementActionSetPart2));
	const PlayerAnimateName actionAnimNames[] = {
		PlayerAnimateName::Backward, PlayerAnimateName::Left, 
		PlayerAnimateName::Right, PlayerAnimateName::Forward
	};
	const std::string actionFrameNames[] = {
		"backward", "left", "right", "forward"
	};
	SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();
	std::stringstream ss;
	for (int i = 0; i < 4; ++i) {
		Animation *animation = Animation::create();
		animation->setDelayPerUnit(.5f);
		for (int j = 0; j < 3; ++j) {
			SpriteFrame *frame = SpriteFrame::createWithTexture(
				(i / 2 == 0 ? texPlayerActionSet1 : texPlayerActionSet2),
				Rect(Vec2(j * playerFrameSize.width, (i % 2) * playerFrameSize.height), playerFrameSize)
			);
			ss.clear();
			ss << actionFrameNames[i] << '_' << j;
			spriteFrameCache->addSpriteFrame(frame, ss.str());
			animation->addSpriteFrame(frame);
			ss.str("");
		}
		Animate *anim = Animate::create(animation);
		anim->setDuration(.5f);
		anim->retain();
		_playerAnims.insert(std::pair<ssize_t, Animate *>(h(actionAnimNames[i]), anim));
	}
}

std::string GlobalManager::getSpriteResourcesPathForName(EnumSpriteName name)
{
	auto target = _spriteResourcesPathRecords.find(h(name));
	if (target != _spriteResourcesPathRecords.end())
		return target->second;
	return nullptr;
}

Rect GlobalManager::getNumFontSetRectangleByChar(const char &c)
{
	auto target = _numFontSetRectangles.find(c);
	if (target != _numFontSetRectangles.end())
		return target->second;
	return Rect::ZERO;
}

cocos2d::Animate *GlobalManager::getAnimateForName(PlayerAnimateName name)
{
	auto target = _playerAnims.find(h(name));
	if (target != _playerAnims.end())
		return target->second;
	return nullptr;
}

void GlobalManager::ExitGame()
{
	_ccDirector->end();
}

std::unordered_map<ssize_t, std::string> &
GlobalManager::getSpriteResourcesPathRecords()
{
	return _spriteResourcesPathRecords;
}

Rect GlobalManager::getAbcFontSetRectangleByChar(const char &c)
{
	auto target = _abcFontSetRectangles.find(c);
	if (target != _abcFontSetRectangles.end())
		return target->second;
	return Rect::ZERO;
}
