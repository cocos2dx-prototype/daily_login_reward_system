//
//  PopupDailyReward.cpp
//  DailyLoginRewardSystem
//
//  Created by hoangbui-fe on 23/6/26.
//
//

#include "PopupDailyReward.hpp"
#include "GameConfig.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

PopupDailyReward::PopupDailyReward() {

}

PopupDailyReward::~PopupDailyReward() {

}

PopupDailyReward* PopupDailyReward::createPopupDailyReward() {
	auto pRet = new PopupDailyReward();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool PopupDailyReward::init() {
	if (!Node::init()) {
		return false;
	}

	{
		this->initValues();
		this->initObjects();
	}
    
	return true;
}

void PopupDailyReward::onEnter() {
	Node::onEnter();

}

void PopupDailyReward::onExit() {
	Node::onExit();
	
}

#pragma mark - ### Private Methods ###
void PopupDailyReward::initValues() {
	_winSize = Director::getInstance()->getWinSize();
	_center = _winSize * 0.5f;
}

void PopupDailyReward::initObjects() {
    // Overlay
    {
        auto pOverlay = LayerColor::create(Color4B(0, 0, 0, 128), this->_winSize.width, this->_winSize.height);
        this->addChild(pOverlay, kChildTagOverlay, kChildTagOverlay);
    }
    
    // Panel
    {
        auto pPanel = LayerColor::create(Color4B(45, 56, 79, 255), 600, 400);
        this->addChild(pPanel, kChildTagPanel);
        pPanel->setPosition(Vec2(_center.x - 300, _center.y - 200));
        _pPanel = pPanel;
    }
    
    // Title
    {
        auto pLabel = Label::createWithTTF("REWARD CLAIMED!", "fonts/arial.ttf", 30.0f);
        _pPanel->addChild(pLabel, kChildTagRewardName);
        pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pLabel->setPosition(Vec2(300.0f, 400 - 20.0f - pLabel->getBoundingBox().size.height * 0.5f));
    }
    
    // Button
    {
        auto pBtn = ui::Button::create("ExitButton.png", "ExitButton.png", "", ui::Widget::TextureResType::LOCAL);
        _pPanel->addChild(pBtn, kChildTagBtnOk);
        pBtn->setPosition(Vec2(300.0f, 20.0f + pBtn->getBoundingBox().size.height * 0.5f));
        pBtn->addClickEventListener([this](Ref* pSender){
            this->runAction(RemoveSelf::create());
        });
    }
}

#pragma mark - ### Public Methods ###
void PopupDailyReward::buildNormalRewardUI(DailyRewardConfig rewardCfg) {
    {
        std::string str = StringUtils::format("%s x%d", rewardCfg.rewardName.c_str(), rewardCfg.rewardAmount);
        auto pLabel = Label::createWithTTF(str, "fonts/arial.ttf", 20.0f);
        _pPanel->addChild(pLabel, kChildTagRewardName);
        pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pLabel->setPosition(Vec2(300.0f, 400 - 150.0f));
    }
}

void PopupDailyReward::buildChestRewardUI(std::vector<ChestRewardConfig> chestRewards) {
    float posY = 400 - 100.0f;
    for (size_t i = 0; i < chestRewards.size(); i++) {
        {
            auto rewardCfg = chestRewards.at(i);
            std::string str = StringUtils::format("%s x%d", rewardCfg.rewardType.c_str(), rewardCfg.rewardAmount);
            auto pLabel = Label::createWithTTF(str, "fonts/arial.ttf", 20.0f);
            _pPanel->addChild(pLabel, kChildTagRewardName);
            pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            pLabel->setPosition(Vec2(300.0f, posY));
        }
        
        posY -= 50.0f;
    }
}

