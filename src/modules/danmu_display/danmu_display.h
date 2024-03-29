#ifndef DANMUKU_MODULES_DANMU_DISPLAY_DANMU_DISPLAY_H_
#define DANMUKU_MODULES_DANMU_DISPLAY_DANMU_DISPLAY_H_

#include <QHBoxLayout>
#include <QListWidget>
#include <QPointer>
#include <QPushButton>
#include <QTimer>

#include "danmu_config.h"
#include "danmu_loader.h"
#include "danmu_panel.h"
#include "danmu_window.h"
#include "module.h"

const int UPDATE_FOLLOWERS_COUNT_INTERVAL_MS = 30000;

class DanmuDisplay : public Module {
    Q_OBJECT
    MODULE

   public:
    DanmuDisplay(Danmuku *parent);
    ~DanmuDisplay();

   private:
    void setLockPosition(const bool &on);

   public slots:
    void enable();
    void disable();
    void startDisplay();
    void startPanel();
    void stop();

    void applyConfig();
    void toggleLockPosition();

    void recvDanmu(const int uid, const QString &username, const QString &text,
                   const bool isAdmin, const bool isVIP,
                   const int userGuardLevel);
    void recvGift(const int uid, const QString &username,
                  const QString &giftName, const int giftCount);

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