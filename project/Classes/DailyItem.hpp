//
//  DailyItem.hpp
//  DailyLoginRewardSystem
//
//  Created by hoangbui-fe on 22/6/26.
//
//

#ifndef DailyItem_hpp
#define DailyItem_hpp

#include "cocos2d.h"
#include "GameDefine.h"

class DailyItem : public cocos2d::Node {
public:
	DailyItem();
	~DailyItem();

public:
	enum ChildTag {
        None = 0,
        kChildTagDrawNode,
        kChildTagDay,
        kChildTagRewardName,
        kChildTagRewardAmount,
	};

public:
	static DailyItem* createDailyItem();

private:
	virtual bool init();
	virtual void update(float delta);
	virtual void onEnter();
	virtual void onExit();
    
    void enableTouch();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

#pragma mark - ### Class Attributes ###
private:
	cocos2d::Size _winSize;
	cocos2d::Vec2 _center;
    
    cocos2d::LayerColor* _pBackground = nullptr;
    RewardState _state = RewardState::Locked;
    int _day = 0;
    std::function<void(int)> _callback;

public:

#pragma mark - ### Class Methods ###
private:
	void initValues();
	void initObjects();

public:
    void setup(DailyItemConfig itemConfig);
    void updateUI(DailyItemConfig itemConfig);
    void setClaimCallback(const std::function<void(int)>& callback);
};

#endif /* DailyItem_hpp */
