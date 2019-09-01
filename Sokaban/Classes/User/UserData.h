
#pragma once

#ifndef __USER_DATA_H__
#define __USER_DATA_H__

#include <json/document.h>

class UserData
{
	using string = std::string;
public:
	UserData();
	~UserData();

public:
	static UserData *getInstance();

	static void setDelegate(UserData *delegate);

	bool init();
	bool IsExistUserData();
	bool saveDefaultUserData();
	bool deleteUserDataFile();

	bool save();

	void setCurrentLevel(const int levelNum);
	int getCurrentLevel() const;

private:
	static UserData *_inst;

	cocos2d::FileUtils *_ccFileUtils;

	string _userDataSaveFilePath;
	string _userDataSaveDictPath;
	static const string _userDataTemplateFilePathWithRes;

	cocos2d::Data _data;
	rapidjson::Document _dataDoc;

	int _currentLevel;
};

#endif // !__USER_DATA_H__