//
//  GameScene.hpp
//  MyGame
//
//  Created by hoangbui-fe on 22/6/26.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "DailyItem.hpp"
#include "DailyRewardManager.hpp"

class GameScene : public cocos2d::Scene {
public:
	GameScene();
	~GameScene();

public:
	enum ChildTag {
        None = 0,
        kChildTagDailyItem,
	};

public:
	static cocos2d::Scene* createGameScene();

private:
	virtual bool init();
	virtual void update(float delta);
	virtual void onEnter();
	virtual void onExit();

#pragma mark - ### Class Attributes ###
private:
	cocos2d::Size _winSize;
	cocos2d::Vec2 _center;
	cocos2d::Rect _safeArea;
	float _scaleRatio;
    
    std::unique_ptr<DailyRewardManager> _dailyRewardManager;
    cocos2d::Vector<DailyItem*> _dailyItems;

public:

#pragma mark - ### Class Methods ###
private:
	void initValues();
	void initObjects();

    void _onRewardTouched(int day);
    void _refreshUI();
public:

};

#endif /* GameScene_hpp */
