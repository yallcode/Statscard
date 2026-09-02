#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <chrono>
#include <string>

struct LevelStats {
    std::string levelName;
    std::string creatorName;
    int attempts = 0;
    int secondsSpent = 0;
    std::chrono::system_clock::time_point timestamp;
};

class StatsCardPopup : public geode::Popup {
protected:
    LevelStats m_stats;
    cocos2d::CCNode* m_cardLayer = nullptr;

    bool init(LevelStats const& stats);
    void buildCard();
    void onSaveImage(cocos2d::CCObject* sender);

    std::string formatTime(int totalSeconds) const;
    std::string formatDate() const;
    std::string calculateGrade(int attempts) const;

public:
    static StatsCardPopup* create(LevelStats const& stats);
};
