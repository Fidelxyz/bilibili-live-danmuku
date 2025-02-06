#ifndef DANMUKU_MODULES_DANMU_DISPLAY_DANMU_DISPLAY_H_
#define DANMUKU_MODULES_DANMU_DISPLAY_DANMU_DISPLAY_H_

#include <QHBoxLayout>
#include <QPointer>
#include <QPushButton>
#include <QTimer>

#include "danmu_config.h"
#include "danmu_loader.h"
#include "danmu_panel.h"
#include "danmu_window.h"
#include "module.h"

constexpr int UPDATE_FOLLOWERS_COUNT_INTERVAL_MS = 30000;

class DanmuDisplay final : public Module {
    Q_OBJECT
    MODULE

   public:
    explicit DanmuDisplay(Danmuku *parent);
    ~DanmuDisplay() override;

   private:
    void setLockPosition(const bool &on);

   public slots:
    void enable() const;
    void disable();
    void startDisplay();
    void startPanel();
    void stop();

    void applyConfig();
    void toggleLockPosition();

    void recvDanmu(int uid, const QString &username, const QString &text,
                   bool isAdmin, bool isVIP, int userGuardLevel);
    void recvGift(int uid, const QString &username, const QString &giftName,
                  int giftCount);

    void updateViewersCount(const int &viewersCount);
    void updateFollowersCount(const int &followersCount);

   private:
    // submodule
    QPointer<DanmuWindow> window;
    QPointer<DanmuLoader> danmuLoader;
    QPointer<DanmuLoader> giftLoader;
    QPointer<DanmuPanel> panel;
    DanmuConfig *config;

    // Module UI
    QPushButton *btn_startDisplay;
    QPushButton *btn_startPanel;
    QPushButton *btn_toggleLockPosition;

    QTimer *updateFollowersCountTimer;
    QString danmuContentFormat;
    QString giftContentFormat;
};

#endif  // DANMUKU_MODULES_DANMU_DISPLAY_DANMU_DISPLAY_H_