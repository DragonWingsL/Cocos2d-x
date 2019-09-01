
#include <cocos2d.h>
#include <cocostudio/CocoStudio.h>

#include "CommonMacro.h"
#include "GlobalManager.h"
#include "AudioUtils.h"

USING_NS_CC;

AudioUtils *AudioUtils::_inst = new AudioUtils();

AudioUtils::AudioUtils()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	_audioNames = {
		{h(AudioName::Touch), "audio/touch.ogg"},
		{h(AudioName::Drop), "audio/drop.ogg"},
		{h(AudioName::Crate), "audio/crate.ogg"},
		{h(AudioName::Completed), "audio/completed.ogg"},
	};
#else
	_audioNames = {
		{h(AudioName::Touch), "audio/touch.mp3"},
		{h(AudioName::Drop), "audio/drop.mp3"},
		{h(AudioName::Crate), "audio/crate.mp3"},
		{h(AudioName::Completed), "audio/completed.mp3"},
	};
#endif
}

AudioUtils *AudioUtils::getInstance()
{
	return _inst;
}

void AudioUtils::play(AudioName audioName)
{
	auto manager = GlobalManager::getInstance();
	if (!manager->IsEnableSound) {
		return;
	}
	else if (!manager->IsEnableMusic) {
		if (audioName == AudioName::Crate)
			return;
	}
	auto target = _audioNames.find(h(audioName));
	if (target == _audioNames.end())
		return;
	_ccSimpleAudioEngine->playEffect(target->second.c_str());
}

std::unordered_map<ssize_t, std::string>& AudioUtils::getAudioPreloadResources()
{
	return _audioNames;
}

void AudioUtils::init()
{
	_ccSimpleAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
}

