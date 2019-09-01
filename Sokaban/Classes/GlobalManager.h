
#pragma once

#ifndef __GLOBAL_MANAGER_H__
#define __GLOBAL_MANAGER_H__

class UserData;
class LevelData;

class GlobalManager
{
	using string = std::string;
	using Rect = cocos2d::Rect;
public:


	enum EnumSpriteName {
		Empty,

		AbcFontSetImage,
		NumFontSetImage,

		MainMenuBackground,
		MainMenuTitle,
		MainMenuPlayBtn,
		MainMenuMusicBtnNormal,
		MainMenuMusicBtnClicked,
		MainMenuSoundBtnNormal,
		MainMenuSoundBtnClicked,
		MainMenuInfoBtnNormal,
		MainMenuInfo,
		MainMenuInfoCloseBtnNormal,

		LevelBackground,
		LevelLockCrate,
		LevelUnlockCrate,
		LevelBackBtnNormal,
		LevelPageBtnNormal,
		LevelPageBtnSelected,

		GameBackgroundTile,
		GameMenuBackground,
		GameMenuBackBtnSmall,
		GameMenuBackBtnMiddle,
		GameMenuRepentBtnNormal,
		GameMenuRestartBtnNormal,
		GameLevelWall,
		GameLevelFloor,
		GameLevelEndPoint,
		GameLevelCrateNormal,
		GameLevelCrateTriggerOfEndPoint,
		GamePlayerElementActionSetPart1,
		GamePlayerElementActionSetPart2,
		GameControl,
		GameOverlap,
		GameNextLevelBtnNormal,
	};

	enum PlayerAnimateName {
		Forward, Backward,
		Left, Right
	};

private:
	GlobalManager();
	~GlobalManager();
	
private:
	static GlobalManager *_inst;

	const string _levelDataFilePath;

	cocos2d::FileUtils *_ccFileUtils;
	cocos2d::Director *_ccDirector;
	UserData *_userData;
	LevelData *_levelData;
	cocos2d::Scene *_currentScene;

	std::unordered_map<ssize_t, string> _spriteResourcesPathRecords;
	std::unordered_map<char, Rect> _abcFontSetRectangles;
	std::unordered_map<char, Rect> _numFontSetRectangles;
	std::unordered_map<ssize_t, cocos2d::Animate *> _playerAnims;

public:
	static GlobalManager *getInstance();
	UserData *getUserData() const;
	LevelData *getLevelData() const;
	
	void init();
	void onLoadCompletedCallback();

	string getSpriteResourcesPathForName(EnumSpriteName name);
	std::unordered_map<ssize_t, string> &getSpriteResourcesPathRecords();

	Rect getAbcFontSetRectangleByChar(const char &c);
	Rect getNumFontSetRectangleByChar(const char &c);

	cocos2d::Animate *getAnimateForName(PlayerAnimateName name);
	void ExitGame();

public:
	bool IsEnableSound;

	bool IsEnableMusic;

	using Vec2 = cocos2d::Vec2;
	using Size = cocos2d::Size;

	Vec2 VisibleOriginPos;
	Size VisibleSize;
	Vec2 VisibleCenterPos;
	Vec2 LevelLeftTopPos;
	Size CellSize;

	int HorizontalCellCount;
	int VerticalCellCount;

};

#endif // !__GLOBAL_MANAGER_H__