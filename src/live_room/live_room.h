#ifndef LIVE_ROOM_H
#define LIVE_ROOM_H

#include <QObject>
#include <QThread>

#include "protocal.h"

class LiveRoom : public QObject {
    Q_OBJECT

   public:
    LiveRoom(const int roomID);
    ~LiveRoom();

   private:
    QJsonObject requestDanmuInfo();
    int requestUid();

   signals:
    void updateFollowersCount(const int& followersCount);

   public slots:
    void slotUpdateFollowersCount();

   public:
    Protocal* protocal;

   private:
    int roomID;
    int uid;
    QThread protocalThread;
    QTimer* updateFollowersCountTimer;
};

#endif