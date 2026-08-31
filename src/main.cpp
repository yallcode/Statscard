#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <chrono>
#include "StatsCardPopup.hpp"

using namespace geode::prelude;

class $modify(StatsCardPlayLayer, PlayLayer) {
    struct Fields {
        std::chrono::steady_clock::time_point m_attemptStart;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }
        m_fields->m_attemptStart = std::chrono::steady_clock::now();
        return true;
    }

    // Restarting a level (death, or manual reset) begins a new attempt —
    // reset the clock so "time spent" reflects the attempt that finishes.
    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->m_attemptStart = std::chrono::steady_clock::now();
    }

    void levelComplete() {
        PlayLayer::levelComplete();

        if (!Mod::get()->getSettingValue<bool>("show-stats-card")) {
            return;
        }

        auto level = this->m_level;
        if (!level) return;

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            now - m_fields->m_attemptStart
        ).count();

        LevelStats stats;
        stats.levelName = level->m_levelName;
        stats.creatorName = level->m_creatorName;
        // level->m_attempts is GD's own attempt counter for this level.
        // Field name has been stable across recent bindings, but worth
        // confirming against your installed Geode SDK version.
        stats.attempts = level->m_attempts;
        stats.secondsSpent = static_cast<int>(elapsed);
        stats.timestamp = std::chrono::system_clock::now();

        StatsCardPopup::create(stats)->show();
    }
};
