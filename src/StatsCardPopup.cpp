#include "StatsCardPopup.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace geode::prelude;

StatsCardPopup* StatsCardPopup::create(LevelStats const& stats) {
    auto ret = new StatsCardPopup();
    if (ret->init(stats)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool StatsCardPopup::init(LevelStats const& stats) {
    if (!Popup::init(280.f, 190.f)) {
        return false;
    }

    m_stats = stats;
    this->setTitle("Level Complete!");

    m_cardLayer = CCNode::create();
    m_cardLayer->setPosition({ m_mainLayer->getContentSize().width / 2, m_mainLayer->getContentSize().height / 2 - 10.f });
    m_mainLayer->addChild(m_cardLayer);

    this->buildCard();

    auto saveSpr = ButtonSprite::create("Save Image", "goldFont.fnt", "GJ_button_01.png", 0.8f);
    auto saveBtn = CCMenuItemSpriteExtra::create(
        saveSpr, this, menu_selector(StatsCardPopup::onSaveImage)
    );
    auto menu = CCMenu::create();
    menu->addChild(saveBtn);
    menu->setPosition({ m_mainLayer->getContentSize().width / 2, 22.f });
    m_mainLayer->addChild(menu);

    if (Mod::get()->getSettingValue<bool>("auto-save-image")) {
        this->onSaveImage(nullptr);
    }

    return true;
}

void StatsCardPopup::buildCard() {
    float cardWidth = 240.f;
    float y = 60.f;

    auto nameLabel = CCLabelBMFont::create(m_stats.levelName.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.55f);
    nameLabel->setPosition({ 0.f, y });
    if (nameLabel->getScaledContentWidth() > cardWidth) {
        nameLabel->setScale(cardWidth / nameLabel->getContentSize().width);
    }
    m_cardLayer->addChild(nameLabel);
    y -= 22.f;

    auto creatorStr = "by " + (m_stats.creatorName.empty() ? std::string("-") : m_stats.creatorName);
    auto creatorLabel = CCLabelBMFont::create(creatorStr.c_str(), "chatFont.fnt");
    creatorLabel->setScale(0.5f);
    creatorLabel->setPosition({ 0.f, y });
    creatorLabel->setColor({ 170, 170, 170 });
    m_cardLayer->addChild(creatorLabel);
    y -= 26.f;

    auto attemptsLabel = CCLabelBMFont::create(
        (std::to_string(m_stats.attempts) + " attempts").c_str(), "chatFont.fnt"
    );
    attemptsLabel->setScale(0.55f);
    attemptsLabel->setPosition({ -80.f, y });
    m_cardLayer->addChild(attemptsLabel);

    auto timeLabel = CCLabelBMFont::create(formatTime(m_stats.secondsSpent).c_str(), "chatFont.fnt");
    timeLabel->setScale(0.55f);
    timeLabel->setPosition({ 0.f, y });
    m_cardLayer->addChild(timeLabel);

    auto grade = calculateGrade(m_stats.attempts);
    auto gradeLabel = CCLabelBMFont::create(grade.c_str(), "bigFont.fnt");
    gradeLabel->setScale(0.6f);
    gradeLabel->setPosition({ 80.f, y });
    gradeLabel->setColor({ 255, 215, 0 });
    m_cardLayer->addChild(gradeLabel);
    y -= 26.f;

    auto dateLabel = CCLabelBMFont::create(formatDate().c_str(), "chatFont.fnt");
    dateLabel->setScale(0.45f);
    dateLabel->setPosition({ 0.f, y });
    dateLabel->setColor({ 150, 150, 150 });
    m_cardLayer->addChild(dateLabel);

    auto watermark = CCLabelBMFont::create("vibemodded", "chatFont.fnt");
    watermark->setScale(0.35f);
    watermark->setAnchorPoint({ 1.f, 0.f });
    watermark->setPosition({ cardWidth / 2, -70.f });
    watermark->setColor({ 120, 120, 120 });
    watermark->setOpacity(160);
    m_cardLayer->addChild(watermark);
}

void StatsCardPopup::onSaveImage(CCObject*) {
    auto size = CCSizeMake(320.f, 220.f);
    auto rt = CCRenderTexture::create((int)size.width, (int)size.height);

    auto oldPos = m_cardLayer->getPosition();
    m_cardLayer->setPosition({ size.width / 2, size.height / 2 - 10.f });

    rt->begin();
    m_cardLayer->visit();
    rt->end();

    m_cardLayer->setPosition(oldPos);

    auto saveDir = Mod::get()->getSaveDir() / "cards";
    std::error_code ec;
    std::filesystem::create_directories(saveDir, ec);

    std::ostringstream filename;
    filename << "card_" << std::time(nullptr) << ".png";
    auto path = saveDir / filename.str();

    rt->saveToFile(path.string().c_str(), cocos2d::kCCImageFormatPNG);

    Notification::create("Saved to " + path.string(), NotificationIcon::Success)->show();
}

std::string StatsCardPopup::formatTime(int totalSeconds) const {
    int mins = totalSeconds / 60;
    int secs = totalSeconds % 60;
    std::ostringstream oss;
    oss << mins << ":" << std::setfill('0') << std::setw(2) << secs;
    return oss.str();
}

std::string StatsCardPopup::formatDate() const {
    auto time = std::chrono::system_clock::to_time_t(m_stats.timestamp);
    std::tm tmStruct{};
#ifdef _WIN32
    localtime_s(&tmStruct, &time);
#else
    localtime_r(&time, &tmStruct);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tmStruct, "%b %d, %Y");
    return oss.str();
}

std::string StatsCardPopup::calculateGrade(int attempts) const {
    if (attempts <= 1) return "S";
    if (attempts <= 5) return "A";
    if (attempts <= 15) return "B";
    if (attempts <= 40) return "C";
    return "D";
}
