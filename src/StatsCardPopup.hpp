#pragma once

#include <Geode/Geode.hpp>
#include <chrono>
#include <string>

// Plain data bag we hand to the popup. Filled in main.cpp from the level
// that was just completed.
struct LevelStats {
    std::string levelName;
    std::string creatorName;
    int attempts = 0;
    int secondsSpent = 0;
    std::chrono::system_clock::time_point timestamp;
};

class StatsCardPopup : public geode::Popup<LevelStats const&> {
protected:
    LevelStats m_stats;
    cocos2d::CCNode* m_cardLayer = nullptr;

    bool setup(LevelStats const& stats) override;

    // Builds the visual card (background, labels, watermark) inside m_cardLayer
    // so the same node tree can be re-used for the on-screen popup AND the
    // rendered-to-PNG export.
    void buildCard();

    void onSaveImage(cocos2d::CCObject* sender);

    std::string formatTime(int totalSeconds) const;
    std::string formatDate() const;
    std::string calculateGrade(int attempts) const;

public:
    static StatsCardPopup* create(LevelStats const& stats);
};
