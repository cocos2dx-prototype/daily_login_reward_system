//
//  PopupDailyReward.hpp
//  DailyLoginRewardSystem
//
//  Created by hoangbui-fe on 23/6/26.
//
//

#ifndef PopupDailyReward_hpp
#define PopupDailyReward_hpp

#include "cocos2d.h"
#include "GameDefine.h"

class PopupDailyReward : public cocos2d::Node {
public:
	PopupDailyReward();
	~PopupDailyReward();

public:
	enum ChildTag {
        None = 0,
        kChildTagOverlay,
        kChildTagPanel,
        kChildTagTitle,
        kChildTagRewardName,
        kChildTagRewardAmount,
        kChildTagBtnOk,
	};

public:
	static PopupDailyReward* createPopupDailyReward();

private:
	virtual bool init();
    virtual void onEnter();
    virtual void onExit();

#pragma mark - ### Class Attributes ###
private:
	cocos2d::Size _winSize;
	cocos2d::Vec2 _center;
    
    cocos2d::LayerColor* _pPanel;

public:

#pragma mark - ### Class Methods ###
private:
	void initValues();
	void initObjects();

public:
    void buildNormalRewardUI(DailyRewardConfig rewardCfg);
    void buildChestRewardUI(std::vector<ChestRewardConfig> chestRewards);
};

#endif /* PopupDailyReward_hpp */
