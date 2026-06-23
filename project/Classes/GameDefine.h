//
//  GameDefine.h
//  DailyLoginRewardSystem-desktop
//
//  Created by hoangbui-fe on 22/6/26.
//

#ifndef GameDefine_h
#define GameDefine_h

enum class RewardState
{
    Locked,     // Gray
    Claimed,    // Green
    Claimable   // Orange
};

struct DailyRewardConfig
{
    int day = 0;

    std::string rewardId;
    std::string rewardType;
    std::string rewardName;

    int rewardAmount = 0;

    std::string icon;
};

struct DailyItemConfig
{
    DailyRewardConfig reward;

    RewardState state = RewardState::Locked;
};

struct ChestRewardConfig {
    std::string rewardType;
    int rewardAmount;
};

#endif /* GameDefine_h */
