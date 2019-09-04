
#include "LayerSetting.h"
#include "LayerMenu.h"

void LayerSetting::onEnter()
{
	Layer::onEnter();
	setPosition(originPos);

	//添加背景图片
	Util::addBackground("bg.png", this);
	Util::addBackground("Set_Music.png", this);
	MenuItem* item = Util::createButtonBk("backA.png", "backB.png", this);
	item->setScale(2.0f);
	Size realSize = Size(item->getContentSize()*item->getScale());
	item->setPosition(Vec2(winSize.width / 2 - realSize.width / 2, -winSize.height / 2 + realSize.height / 2));
	item->setTarget(this, menu_selector(LayerSetting::Back));

	//添加音效控制
	ControlSlider* effect = ControlSlider::create("sliderTrack.png", "sliderProgress.png", "sliderThumb.png");
	addChild(effect);
	effect->setPosition(Vec2(winSize.width/2 + 80, winSize.height/2 + 70));
	effect->setMaximumValue(1);
	effect->setMinimumValue(0);
	//默认的音量
	float value = UserDefault::getInstance()->getFloatForKey("EffectValue", 0.5f);
	effect->setValue(value);

	effect->addTargetWithActionForControlEvents(this, cccontrol_selector(LayerSetting::Effect), Control::EventType::VALUE_CHANGED);

	ControlSlider* music = ControlSlider::create("sliderTrack.png", "sliderProgress.png", "sliderThumb.png");
	addChild(music);
	music->setPosition(Vec2(winSize.width/2, winSize.height/2));
	music->setMaximumValue(1);
	music->setMinimumValue(0);

	value = UserDefault::getInstance()->getFloatForKey("MusicValue", 0.5f);
	music->setValue(value);
	music->addTargetWithActionForControlEvents(this, cccontrol_selector(LayerSetting::Music), Control::EventType::VALUE_CHANGED);
}

void LayerSetting::Effect(Ref* sender, Control::EventType)
{
	//当音量发生改变就记录
	ControlSlider* slider = (ControlSlider*)sender;
	float value = slider->getValue();
	UserDefault::getInstance()->setFloatForKey("EffectValue", value);
	UserDefault::getInstance()->flush();
}

void LayerSetting::Music(Ref* sender, Control::EventType)
{
	//当音量发生改变就记录
	ControlSlider* slider = (ControlSlider*)sender;
	float value = slider->getValue();
	UserDefault::getInstance()->setFloatForKey("MusicValue", value);
	UserDefault::getInstance()->flush();
}

void LayerSetting::Back(Ref* )
{
	switchScene(LayerMenu);
}

void LayerSetting::onExit()
{
	Layer::onExit();
}