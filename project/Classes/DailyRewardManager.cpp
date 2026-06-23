//
//  DailyRewardManager.cpp
//  DailyLoginRewardSystem-desktop
//
//  Created by hoangbui-fe on 22/6/26.
//

#include "DailyRewardManager.hpp"
#include "json/document.h"
#include <iomanip>
#include "GameConfig.h"

USING_NS_CC;

namespace
{
    const char* KEY_CURRENT_DAY = "daily_reward_current_day";
    const char* KEY_LAST_DATE   = "daily_reward_last_date";
}

DailyRewardManager::DailyRewardManager() {
    auto ud = cocos2d::UserDefault::getInstance();

    ud->deleteValueForKey(KEY_CURRENT_DAY);
    ud->deleteValueForKey(KEY_LAST_DATE);

    ud->flush();
}

DailyRewardManager::~DailyRewardManager() {
    
}

bool DailyRewardManager::initializeFromJson(
    const std::string& filePath) {
    std::string jsonStr =
        FileUtils::getInstance()
            ->getStringFromFile(filePath);

    if (jsonStr.empty()) {
        CCLOG("DailyRewardManager: JSON empty");
        return false;
    }

    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("DailyRewardManager: JSON parse failed");
        return false;
    }

    const rapidjson::Value& root = doc["daily_login_rewards"];
    const rapidjson::Value& rewards = root["rewards"];

    _rewards.clear();

    for (rapidjson::SizeType i = 0; i < rewards.Size(); i++)
    {
        const rapidjson::Value& r = rewards[i];

        DailyRewardConfig cfg;
        cfg.day = r["day"].GetInt();
        cfg.rewardId = r["reward_id"].GetString();
        cfg.rewardType = r["reward_type"].GetString();
        cfg.rewardName = r["reward_name"].GetString();
        cfg.rewardAmount = r["reward_amount"].GetInt();
        cfg.icon = r["icon"].GetString();

        _rewards.push_back(cfg);
    }

    load();
    validateProgress();
    
    // Parse Chest Reward
    const rapidjson::Value& masterChestRewards = doc["master_chest_rewards"];
    _chestRewards.clear();
    
    for (rapidjson::SizeType i = 0; i < masterChestRewards.Size(); i++) {
        const auto& chestJson = masterChestRewards[i];
        const auto& possibleRewards = chestJson["possible_rewards"];
        for (rapidjson::SizeType j = 0; j < possibleRewards.Size(); j++)
        {
            const rapidjson::Value& r = possibleRewards[j];

            ChestRewardConfig cfg;
            cfg.rewardType = r["type"].GetString();
            cfg.rewardAmount = r["amount"].GetInt();
            _chestRewards.push_back(cfg);
        }
    }

    return true;
}

void DailyRewardManager::load() {
    auto ud = UserDefault::getInstance();

    _claimedDayCount =
        ud->getIntegerForKey(KEY_CURRENT_DAY, 0);

    _lastClaimDate =
        ud->getStringForKey(KEY_LAST_DATE, "");
}

void DailyRewardManager::save() {
    auto ud = UserDefault::getInstance();

    ud->setIntegerForKey(KEY_CURRENT_DAY, _claimedDayCount);
    ud->setStringForKey(KEY_LAST_DATE, _lastClaimDate);

    ud->flush();
}

bool DailyRewardManager::canClaimToday() const {
    if (_lastClaimDate.empty()) {
        return true;
    }

    const int diff =
        getDaysDifference(
            _lastClaimDate,
            getTodayDate());

    if (diff < 0) {
        return false;
    }

    return diff >= 1;
}

bool DailyRewardManager::claimReward() {
    if (!canClaimToday()) {
        return false;
    }

    _lastClaimDate = getTodayDate();

    _claimedDayCount++;

    if (_claimedDayCount > (int)_rewards.size()) {
        _claimedDayCount = 0;
    }

    save();
    return true;
}

int DailyRewardManager::getClaimedDayCount() const {
    return _claimedDayCount;
}

std::vector<DailyItemConfig>
DailyRewardManager::buildRewardConfigs() {
    std::vector<DailyItemConfig> result;

    bool canClaim = canClaimToday();
    int claimableDay = _claimedDayCount + 1;
    
    for (const auto& reward : _rewards) {
        DailyItemConfig item;
        item.reward = reward;

        if (reward.day <= _claimedDayCount) {
            item.state = RewardState::Claimed;
        } else if (reward.day == claimableDay && canClaim) {
            item.state = RewardState::Claimable;
        } else {
            item.state = RewardState::Locked;
        }

        result.push_back(item);
    }

    return result;
}

DailyRewardConfig DailyRewardManager::getRewardConfigByDay(int day) {
    DailyRewardConfig reward;
    
    for (size_t i = 0; i < _rewards.size(); i++) {
        auto r = _rewards.at(i);
        if (r.day == day) {
            return r;
        }
    }
    
    return reward;
}

std::vector<ChestRewardConfig> DailyRewardManager::getChestRewards() {
    return _chestRewards;
}

void DailyRewardManager::validateProgress() {
    if (_lastClaimDate.empty()) {
        return;
    }

    int diff = getDaysDifference(
        _lastClaimDate,
        getTodayDate());

    if (diff < 0) {
        CCLOG("DailyReward: System clock moved backwards.");
        return;
    }
    
    if (diff > 1) {
        resetProgress();
    }
}

void DailyRewardManager::resetProgress() {
    _claimedDayCount = 0;
    _lastClaimDate.clear();

    save();
}

std::string DailyRewardManager::getTodayDate() const {
    
#if (DEBUG > 0)
    return DEBUG_DATE;
#endif
    std::time_t now = std::time(nullptr);

    std::tm t;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    localtime_s(&t, &now);
#else
    localtime_r(&now, &t);
#endif

    std::ostringstream oss;
    oss << std::put_time(&t, "%Y-%m-%d");

    return oss.str();
}

int DailyRewardManager::getDaysDifference(
    const std::string& oldDate,
    const std::string& newDate) const {
    constexpr int SEC_PER_DAY = 86400;

    std::time_t oldT = parseDate(oldDate);
    std::time_t newT = parseDate(newDate);

    return (int)(std::difftime(newT, oldT) / SEC_PER_DAY);
}

std::time_t DailyRewardManager::parseDate(
    const std::string& date) const {
    std::tm t = {};

    sscanf(date.c_str(),
           "%d-%d-%d",
           &t.tm_year,
           &t.tm_mon,
           &t.tm_mday);

    t.tm_year -= 1900;
    t.tm_mon  -= 1;

    t.tm_hour = 0;
    t.tm_min  = 0;
    t.tm_sec  = 0;

    return std::mktime(&t);
}
