
#pragma once

#ifndef __CRATE_ELEMENT_H__
#define __CRATE_ELEMENT_H__

#include "Level/MoveElement.h"

enum CrateStatus {
	Normal, Trigger
};

class CrateElement : public MoveElement
{
public:
	static CrateElement *create(ElementType type);
	virtual bool initWithType(ElementType type) override;
	void setStatus(CrateStatus status);
	CrateStatus getStatus() const;

protected:
	struct CrateStep : public Step {
		typedef std::function<void(cocos2d::Vec2, CrateStatus)> callbackFunc;
		callbackFunc callback;
		cocos2d::Vec2 position;
		CrateStatus crateStatus;
		CrateStep(callbackFunc callback, cocos2d::Vec2 pos, CrateStatus crateStatus);
		virtual void call() override;
	};

	virtual MoveElement::Step *getStep() override;

protected:
	cocos2d::TextureCache *_ccTexCache;
	CrateStatus _status;

};

#endif // !__CRATE_ELEMENT_H__