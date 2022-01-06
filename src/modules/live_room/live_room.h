#ifndef LIVE_ROOM_H
#define LIVE_ROOM_H

#include <QHBoxLayout>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QThread>

#include "module.h"
#include "protocal.h"

class LiveRoom : public Module {
    Q_OBJECT
    MODULE

   public:
    LiveRoom();
    ~LiveRoom();

   private:
    QJsonObject requestDanmuInfo();
    int requestUid();
    void stop();

   signals:
    void followersCountUpdated(const int& followersCount);

   public slots:
    void start();
    bool isRunning();
    void updateFollowersCount();

   public:
    Protocal* protocal;

   private:
    QLineEdit* input_roomID;
    QPushButton* btn_start;

    int roomID;
    int uid;
    QThread protocalThread;
};

#endif