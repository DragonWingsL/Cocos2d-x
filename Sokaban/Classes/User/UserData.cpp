
#include <cocos2d.h>
#include <json/stringbuffer.h>
#include <json/writer.h>

#include "CommonMacro.h"
#include "GlobalManager.h"
#include "UserData.h"

USING_NS_CC;

const std::string UserData::_userDataTemplateFilePathWithRes = std::string("userData/template.json");
UserData *UserData::_inst = nullptr;

UserData::UserData()
{
	_ccFileUtils = FileUtils::getInstance();
	if (!init()) {
		// Reset user data to try agin.
		deleteUserDataFile();
		do {
			CC_BREAK_IF(init());
		} while (0);
		return;
	}
}

UserData::~UserData()
{
}

bool UserData::init()
{
	do {
		const string writablePath = _ccFileUtils->getWritablePath();
		_userDataSaveDictPath = writablePath + "UserData";
		_userDataSaveFilePath = _userDataSaveDictPath + "/user.json";

		if (!IsExistUserData())
			CC_BREAK_IF(!saveDefaultUserData());

		_data = _ccFileUtils->getDataFromFile(_userDataSaveFilePath);
		CC_BREAK_IF(_data.getSize() <= 0);
		_dataDoc.Parse((char *)_data.getBytes(), _data.getSize());
		CC_BREAK_IF(_dataDoc.HasParseError());

		const rapidjson::Value &obj = _dataDoc["cur_level"];
		CC_BREAK_IF(!obj.IsNumber());
		_currentLevel = obj.GetInt();
		CC_BREAK_IF(_currentLevel <= 0 || _currentLevel > MAX_LEVEL);
		return true;
	} while (0);
	return false;
}

UserData *UserData::getInstance()
{
	return _inst;
}

void UserData::setDelegate(UserData *delegate)
{
	if (_inst)
		delete _inst;

	_inst = delegate;
}

bool UserData::IsExistUserData()
{
	return _ccFileUtils->isFileExist(_userDataSaveFilePath);
}

bool UserData::saveDefaultUserData()
{
	if (IsExistUserData())
		return true;

	do {
		ssize_t s = 0;
		Data data = _ccFileUtils->getDataFromFile(_userDataTemplateFilePathWithRes);
		CC_BREAK_IF(data.getSize() <= 0);

		if (!_ccFileUtils->isDirectoryExist(_userDataSaveDictPath)) {
#if IS_NOT_WIN_OS_PLATFORM
			CC_BREAK_IF(!_ccFileUtils->createDirectory(_userDataSaveDictPath));
#else
			BOOL ret = false;
			ret = ::CreateDirectoryA(_userDataSaveDictPath.c_str(), NULL);
			CC_BREAK_IF(!ret && ERROR_ALREADY_EXISTS != GetLastError());
#endif
		}

		CC_BREAK_IF(!_ccFileUtils->writeDataToFile(data, _userDataSaveFilePath));
		return true;
	} while (0);
	return false;
}

bool UserData::deleteUserDataFile()
{
	do {
		CC_BREAK_IF(!IsExistUserData());
#if IS_NOT_WIN_OS_PLATFORM
		CC_BREAK_IF(!_ccFileUtils->removeFile(_userDataSaveFilePath));
#else
		CC_BREAK_IF(!remove(_userDataSaveFilePath.c_str()));
#endif
		return true;
	} while (0);
	return false;
}

bool UserData::save()
{
	do {
		if (!IsExistUserData()) {
			CC_BREAK_IF(!saveDefaultUserData());
		}

		using rapidjson::StringBuffer;
		using rapidjson::Writer;
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		_dataDoc.Accept(writer);

		_data.fastSet((unsigned char *)sb.GetString(), sb.GetSize());
		CC_BREAK_IF(!_ccFileUtils->writeDataToFile(_data, _userDataSaveFilePath));
		return true;
	} while (0);
	return false;
}

void UserData::setCurrentLevel(const int levelNum)
{
	_currentLevel = levelNum;
	_dataDoc["cur_level"].SetInt(_currentLevel);
}

int UserData::getCurrentLevel() const
{
	return _currentLevel;
}


