#include "danmu_config.h"

#include "global.h"

DanmuConfig::DanmuConfig(QObject *parent)
    : QObject(parent),
      settings(APP_DIR_PATH + CONFIG_DIR_PATH + "/DanmuDisplay.ini",
               QSettings::IniFormat) {
    load();

    qDebug("DanmuConfig loaded");
    qDebug() << "INI file path:" << settings.fileName();
}

void DanmuConfig::load() {
    windowWidth =
        settings.value("Window/WindowWidth", DEFAULT_CONFIG::windowWidth)
            .toInt();
    windowHeight =
        settings.value("Window/WindowHeight", DEFAULT_CONFIG::windowHeight)
            .toInt();
    opacity =
        settings.value("Window/OpacityPercentage", DEFAULT_CONFIG::opacity)
            .toInt();
    borderRadius =
        settings.value("Window/BorderRadius", DEFAULT_CONFIG::borderRadius)
            .toInt();
    lockPosition =
        settings.value("Window/LockPosition", DEFAULT_CONFIG::lockPosition)
            .toBool();
    font = QFont();
    font.fromString(
        settings.value("Font/Font", DEFAULT_CONFIG::font).toString());
    mainColor =
        settings.value("Color/MainColor", DEFAULT_CONFIG::mainColor).toString();
    usernameColor =
        settings.value("Color/UsernameColor", DEFAULT_CONFIG::usernameColor)
            .toString();
    contentColor =
        settings.value("Color/ContentColor", DEFAULT_CONFIG::contentColor)
            .toString();
    backgroundColor =
        settings.value("Color/BackgroundColor", DEFAULT_CONFIG::backgroundColor)
            .toString();
    scrollingSpeed =
        settings.value("Danmu/ScrollingSpeed", DEFAULT_CONFIG::scrollingSpeed)
            .toInt();
    fps = settings.value("Danmu/FPS", DEFAULT_CONFIG::fps).toInt();
    showGift =
        settings.value("Gift/ShowGift", DEFAULT_CONFIG::showGift).toBool();
    giftHeightRatio =
        settings.value("Gift/GiftHeightRatio", DEFAULT_CONFIG::giftHeightRatio)
            .toInt();
}

void DanmuConfig::save() {
    settings.setValue("Window/WindowWidth", windowWidth);
    settings.setValue("Window/WindowHeight", windowHeight);
    settings.setValue("Window/OpacityPercentage", opacity);
    settings.setValue("Window/BorderRadius", borderRadius);
    settings.setValue("Window/LockPosition", lockPosition);
    settings.setValue("Font/Font", font.toString());
    settings.setValue("Color/MainColor", mainColor.name());
    settings.setValue("Color/UsernameColor", usernameColor.name());
    settings.setValue("Color/ContentColor", contentColor.name());
    settings.setValue("Color/BackgroundColor", backgroundColor.name());
    settings.setValue("Danmu/ScrollingSpeed", scrollingSpeed);
    settings.setValue("Danmu/FPS", fps);
    settings.setValue("Gift/ShowGift", showGift);
    settings.setValue("Gift/GiftHeightRatio", giftHeightRatio);

    settings.sync();
}

void DanmuConfig::apply() { emit changed(); }