
#pragma once

#ifndef __COMMON_MACRO_H__
#define __COMMON_MACRO_H__

#include "GlobalManager.h"
#include "platform/CCPlatformMacros.h"

#define IS_NOT_WIN_OS_PLATFORM (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)

#define MAX_LEVEL 70

#define ONE_OVER_TWO (1 / 2.0f)

#define NODE_CENTER_POSITION(__node__) Vec2(__node__->getContentSize() * __node__->getScale() * ONE_OVER_TWO)

#define h(__name__) static_cast<ssize_t>(__name__)

#endif // !__COMMON_MACRO_H__

