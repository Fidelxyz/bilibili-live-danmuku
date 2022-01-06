#ifndef DANMU_DISPLAY_H
#define DANMU_DISPLAY_H

#include <QHBoxLayout>
#include <QListWidget>
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
    DanmuDisplay();
    ~DanmuDisplay();

   private:
    void setOnTop(const bool &on);

   public slots:
    void startDisplay();
    void startPanel();

    void applyConfig();
    void toggleOnTop();

    void recvDanmu(const int &uid, const QString &username, const QString &text,
                   const bool &isAdmin, const bool &isVIP,
                   const int &userGuardLevel);
    void updateViewersCount(const int &viewersCount);
    void updateFollowersCount(const int &followersCount);

   private:
    // submodule
    DanmuWindow *window;
    DanmuLoader *loader;
    DanmuPanel *panel;
    DanmuConfig *config;

    // Module UI
    QPushButton *btn_startDisplay;
    QPushButton *btn_startPanel;
    QPushButton *btn_toggleOnTop;

    QListWidget *danmuList;

    QTimer *updateFollowersCountTimer;
    QString contentFormat;
};

#endif