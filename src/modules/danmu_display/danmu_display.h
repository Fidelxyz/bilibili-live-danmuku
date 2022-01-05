#ifndef DANMU_DISPLAY_H
#define DANMU_DISPLAY_H

#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>

#include "danmu_config.h"
#include "danmu_loader.h"
#include "danmu_panel.h"
#include "danmu_window.h"
#include "module.h"

class DanmuDisplay : public Module {
    Q_OBJECT
    MODULE

   public:
    DanmuDisplay(QWidget *parent);
    ~DanmuDisplay();

   public slots:
    void slotStartDisplay();
    void slotStartPanel();

    void slotRecvDanmu(const int &uid, const QString &username,
                       const QString &text, const bool &isAdmin,
                       const bool &isVIP, const int &userGuardLevel);
    void slotUpdateViewersCount(const int &viewersCount);
    void slotUpdateFollowersCount(const int &followersCount);

    void slotSetConfig();

    void slotSetOnTop(const bool &onTop);

   private:
    void reload();
    void applyConfig();

   private:
    // panel UI
    QPushButton *btn_startDisplay;
    QPushButton *btn_startPanel;

    // submodule
    DanmuWindow *window;
    DanmuLoader *loader;
    DanmuPanel *panel;
    DanmuConfig *config;

    QListWidget *danmuList;

    QString contentFormat;
};

#endif