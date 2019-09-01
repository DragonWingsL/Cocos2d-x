
#pragma once

#ifndef __AUDIO_UTILS_H__
#define __AUDIO_UTILS_H__

#include "SimpleAudioEngine.h"

class AudioUtils
{
public:
	enum AudioName {
		Touch,
		Drop,
		Crate,
		Completed,
	};

public:
	AudioUtils();
	static AudioUtils *getInstance();
	void play(AudioName audioName);
	std::unordered_map<ssize_t, std::string> &getAudioPreloadResources();
	void init();

protected:
	static AudioUtils *_inst;
	std::unordered_map<ssize_t, std::string> _audioNames;
	CocosDenshion::SimpleAudioEngine *_ccSimpleAudioEngine;
};

#endif // !__AUDIO_UTILS_H__