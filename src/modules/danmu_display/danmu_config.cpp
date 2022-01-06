#include "danmu_config.h"

#include "global.h"

DanmuConfig::DanmuConfig(QObject *parent)
    : QObject(parent),
      settings(APP_DIR_PATH + CONFIG_DIR_PATH + "/DanmuDisplay.ini",
               QSettings::IniFormat) {
    windowWidth = settings.value("Window/WindowWidth", 400).toInt();
    windowHeight = settings.value("Window/WindowHeight", 800).toInt();
    opacity = settings.value("Window/OpacityPercentage", 70).toInt();
    onTop = settings.value("Window/OnTop", false).toBool();
    fontSize = settings.value("Font/FontSize", 15).toInt();
    mainColor = settings.value("Color/MainColor", "white").toString();
    usernameColor =
        settings.value("Color/UsernameColor", "lightblue").toString();
    contentColor = settings.value("Color/ContentColor", "white").toString();
    backgroundColor =
        settings.value("Color/BackgroundColor", "black").toString();

    qDebug("DanmuConfig loaded");
    qDebug() << "INI file path:" << settings.fileName();
}

DanmuConfig::~DanmuConfig() {}

void DanmuConfig::save() {
    settings.setValue("Window/WindowWidth", windowWidth);
    settings.setValue("Window/WindowHeight", windowHeight);
    settings.setValue("Window/OpacityPercentage", opacity);
    settings.setValue("Window/OnTop", onTop);
    settings.setValue("Font/FontSize", fontSize);
    settings.setValue("Color/MainColor", mainColor.name());
    settings.setValue("Color/UsernameColor", usernameColor.name());
    settings.setValue("Color/ContentColor", contentColor.name());
    settings.setValue("Color/BackgroundColor", backgroundColor.name());

    settings.sync();
}

void DanmuConfig::apply() { emit changed(); }