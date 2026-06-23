//
//  DailyItem.cpp
//  DailyLoginRewardSystem
//
//  Created by hoangbui-fe on 22/6/26.
//
//

#include "DailyItem.hpp"
#include "StyleGuide.h"

USING_NS_CC;

DailyItem::DailyItem() {

}

DailyItem::~DailyItem() {

}

DailyItem* DailyItem::createDailyItem() {
	auto pRet = new DailyItem();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool DailyItem::init() {
	if (!Node::init()) {
		return false;
	}

	{
		this->initValues();
		this->initObjects();
	}
    
	return true;
}

void DailyItem::update(float delta) {
	Node::update(delta);
}

void DailyItem::onEnter() {
	Node::onEnter();
    enableTouch();
}

void DailyItem::onExit() {
	Node::onExit();
	
}

#pragma mark - ### Private Methods ###
void DailyItem::initValues() {
	_winSize = Director::getInstance()->getWinSize();
	_center = _winSize * 0.5f;
}

void DailyItem::initObjects() {
    auto pLayerColor = LayerColor::create(Color4B::GRAY, DAILY_ITEM_WIDTH, DAILY_ITEM_HEIGHT);
    this->addChild(pLayerColor, kChildTagDrawNode);
    pLayerColor->setPosition(Vec2::ZERO);
    _pBackground = pLayerColor;
    
    this->setContentSize(Size(DAILY_ITEM_WIDTH, DAILY_ITEM_HEIGHT));
}

#pragma mark - Touch Event
void DailyItem::enableTouch() {
    // Enable touch
    {
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(DailyItem::onTouchBegan, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
}

bool DailyItem::onTouchBegan(Touch *touch, Event *unused_event) {
//    if (_state != RewardState::Claim) {
//        return false;
//    }
    
    Vec2 localPos = this->convertToNodeSpace(touch->getLocation());
    Rect rect(0, 0, this->getContentSize().width, this->getContentSize().height);
    
    if (!rect.containsPoint(localPos)) {
        return false;
    }
    
    if (_callback) {
        _callback(_day);
    }
    
    return true;
}

#pragma mark - ### Public Methods ###
void DailyItem::setup(DailyItemConfig itemConfig) {
    DailyRewardConfig reward = itemConfig.reward;
    _day = reward.day;
    
    Vec2 midPos = this->getContentSize() * 0.5f;
    // Label Day
    {
        std::string str = StringUtils::format("Day %d", _day);
        auto pLabel = Label::createWithTTF(str, "fonts/arial.ttf", 20.0f);
        this->addChild(pLabel, kChildTagDay);
        pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pLabel->setPosition(Vec2(midPos.x, this->getContentSize().height - 10.0f - pLabel->getBoundingBox().size.height * 0.5f));
        pLabel->enableBold();
        pLabel->enableOutline(Color4B::BLACK, 1);
    }
    
    // Reward Type
    {
        auto pLabel = Label::createWithTTF(reward.rewardType, "fonts/arial.ttf", 30.0f);
        this->addChild(pLabel, kChildTagRewardName);
        pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pLabel->setPosition(Vec2(midPos));
        pLabel->enableBold();
        pLabel->enableOutline(Color4B::BLACK, 2);
    }
    
    // Reward Amount
    {
        std::string str = StringUtils::format("%d", reward.rewardAmount);
        auto pLabel = Label::createWithTTF(str, "fonts/arial.ttf", 20.0f);
        this->addChild(pLabel, kChildTagRewardAmount);
        pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pLabel->setPosition(Vec2(midPos.x, 60.0f + pLabel->getBoundingBox().size.height * 0.5f));
        pLabel->enableBold();
        pLabel->enableOutline(Color4B::BLACK, 1);
    }
    
    updateUI(itemConfig);
}

void DailyItem::updateUI(DailyItemConfig itemConfig) {
    switch (itemConfig.state) {
        case RewardState::Locked:
            _pBackground->setColor(Color3B::GRAY);
            break;
            
        case RewardState::Claimed:
            _pBackground->setColor(Color3B::GREEN);
            break;
            
        case RewardState::Claimable:
            _pBackground->setColor(Color3B::ORANGE);
            break;
            
        default:
            break;
    }
}

void DailyItem::setClaimCallback(const std::function<void (int)> &callback) {
    _callback = callback;
}

