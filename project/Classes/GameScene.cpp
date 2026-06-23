//
//  GameScene.cpp
//  MyGame
//
//  Created by hoangbui-fe on 22/6/26.
//
//

#include "GameScene.hpp"
#include "StyleGuide.h"

USING_NS_CC;

GameScene::GameScene() {

}

GameScene::~GameScene() {

}

Scene* GameScene::createGameScene() {
	auto pRet = new GameScene();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}

	{
		this->initValues();
		this->initObjects();
	}

	return true;
}

void GameScene::update(float delta) {
	Scene::update(delta);
}

void GameScene::onEnter() {
	Scene::onEnter();

}

void GameScene::onExit() {
	Scene::onExit();
	
}

#pragma mark - ### Private Methods ###
void GameScene::initValues() {
	_winSize = Director::getInstance()->getWinSize();
	_center = _winSize * 0.5f;
	_safeArea = Director::getInstance()->getSafeAreaRect();
}

void GameScene::initObjects() {
    _dailyRewardManager = std::make_unique<DailyRewardManager>();
    _dailyRewardManager->initializeFromJson("daily_rewards_data.json");
    
    std::vector<DailyItemConfig> dailyItemConfigs = _dailyRewardManager->buildRewardConfigs();
    
    float margin = 15.0f;
    float padding = (_winSize.width - (DAILY_ITEM_WIDTH * 7) - (margin * (7 - 1))) * 0.5f;
    float startPosX = padding;
    float posY = _center.y - DAILY_ITEM_HEIGHT * 0.5f;
    for (int i = 0; i < dailyItemConfigs.size(); i++) {
        DailyItemConfig itemConfig = dailyItemConfigs.at(i);
        
        auto dailyItem = DailyItem::createDailyItem();
        this->addChild(dailyItem, kChildTagDailyItem);
        Vec2 pos = Vec2(startPosX, posY);
        dailyItem->setPosition(Vec2(pos));
        startPosX += (DAILY_ITEM_WIDTH + margin);
        
        dailyItem->setup(itemConfig);
        dailyItem->setClaimCallback([this](int day) {
            CCLOG("Click on day %d", day);
            _onRewardTouched(day);
        });
        
        _dailyItems.pushBack(dailyItem);
    }
}

void GameScene::_onRewardTouched(int day) {
    if (_dailyRewardManager->canClaimToday() && _dailyRewardManager->claimReward()) {
        auto reward = _dailyRewardManager->getRewardConfigByDay(day);
        CCLOG("Day = %d", reward.day);
        CCLOG("Reward Name = %s", reward.rewardName.c_str());
        CCLOG("Reward Amount = %d", reward.rewardAmount);
        _refreshUI();
    }
}

void GameScene::_refreshUI() {
    std::vector<DailyItemConfig> dailyItemConfigs = _dailyRewardManager->buildRewardConfigs();
    for (int i = 0; i < dailyItemConfigs.size(); i++) {
        DailyItemConfig itemConfig = dailyItemConfigs.at(i);
        auto dailyItem = _dailyItems.at(i);
        dailyItem->updateUI(itemConfig);
    }
}

#pragma mark - ### Public Methods ###

