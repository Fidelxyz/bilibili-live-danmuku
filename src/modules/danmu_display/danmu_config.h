#ifndef DANMUKU_MODULES_DANMU_DISPLAY_DANMU_CONFIG_H_
#define DANMUKU_MODULES_DANMU_DISPLAY_DANMU_CONFIG_H_

#include <QColor>
#include <QFont>
#include <QSettings>

namespace DEFAULT_CONFIG {
constexpr int windowWidth = 350;
constexpr int windowHeight = 600;
constexpr int opacity = 70;
constexpr int borderRadius = 8;
constexpr bool lockPosition = false;
const auto font = QFont("Microsoft YaHei UI");
const QColor mainColor = "white";
const QColor usernameColor = "lightblue";
const QColor contentColor = "white";
const QColor backgroundColor = "black";
constexpr int scrollingSpeed = 20;
constexpr int fps = 30;
constexpr bool showGift = true;
constexpr int giftHeightRatio = 20;
}  // namespace DEFAULT_CONFIG

class DanmuConfig final : public QObject {
    Q_OBJECT

   public:
    explicit DanmuConfig(QObject *parent = nullptr);
    void load();
    void save();
    void apply();

   signals:
    void changed();

   public:
    // Window

    int windowWidth;
    int windowHeight;
    int opacity;
    int borderRadius;
    bool lockPosition;

    // Font

    QFont font;

    // Color

    QColor mainColor;
    QColor usernameColor;
    QColor contentColor;
    QColor backgroundColor;

    // Danmu

    int scrollingSpeed;
    int fps;

    // Gift

    bool showGift;
    int giftHeightRatio;

   private:
    QSettings settings;
};

#endif  // DANMUKU_MODULES_DANMU_DISPLAY_DANMU_CONFIG_H_