//
//  DailyRewardManager.hpp
//  DailyLoginRewardSystem-desktop
//
//  Created by hoangbui-fe on 22/6/26.
//

#ifndef DailyRewardManager_hpp
#define DailyRewardManager_hpp

#include "cocos2d.h"
#include "GameDefine.h"

class DailyRewardManager {
public:
    DailyRewardManager();
    ~DailyRewardManager();
    
public:
    bool initializeFromJson(const std::string& filePath);
    
    void load();
    void save();
    
    // Core logic
    bool canClaimToday() const;
    bool claimReward();

    int getClaimedDayCount() const;

    // UI helper
    std::vector<DailyItemConfig> buildRewardConfigs();
    DailyRewardConfig getRewardConfigByDay(int day);
    
private:
private:
    void validateProgress();
    void resetProgress();
    
    std::string getTodayDate() const;
    int getDaysDifference(const std::string& oldDate,
                          const std::string& newDate) const;

    std::time_t parseDate(const std::string& date) const;
    
private:
    std::vector<DailyRewardConfig> _rewards;
    int _claimedDayCount = 0;
    std::string _lastClaimDate;
};

#endif /* DailyRewardManager_hpp */
