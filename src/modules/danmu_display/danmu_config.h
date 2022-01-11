#ifndef DANMU_CONFIG_H
#define DANMU_CONFIG_H

#include <QColor>
#include <QFont>
#include <QSettings>

namespace DEFAULT_CONFIG {
const int windowWidth = 350;
const int windowHeight = 600;
const int opacity = 70;
const int borderRadius = 8;
const bool lockPosition = false;
const QFont font = QFont("Microsoft YaHei UI");
const QColor mainColor = "white";
const QColor usernameColor = "lightblue";
const QColor contentColor = "white";
const QColor backgroundColor = "black";
const int scrollingSpeed = 20;
const int fps = 30;
}  // namespace DEFAULT_CONFIG

class DanmuConfig : public QObject {
    Q_OBJECT

   public:
    DanmuConfig(QObject *parent = nullptr);
    ~DanmuConfig();
    void load();
    void save();
    void apply();

   signals:
    void changed();

   public:
    int windowWidth;
    int windowHeight;
    int opacity;
    int borderRadius;
    bool lockPosition;
    QFont font;
    QColor mainColor;
    QColor usernameColor;
    QColor contentColor;
    QColor backgroundColor;
    int scrollingSpeed;
    int fps;

   private:
    QSettings settings;
};

#endif