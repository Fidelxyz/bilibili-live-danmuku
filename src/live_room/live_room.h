#ifndef LIVE_ROOM_H
#define LIVE_ROOM_H

#include <QObject>
#include <QThread>

#include "danmu_display.h"
#include "protocal.h"

class LiveRoom : public QObject {
    Q_OBJECT

   public:
    LiveRoom(const int roomID);
    ~LiveRoom();

   private:
    QJsonObject getInfo();
    void initDisplay();

   signals:
    void startProtocalConnection(const int& roomID,
                                 const QJsonObject& liveRoomInfo);

   private:
    int roomID;

    Protocal* protocal;
    QThread protocalThread;
    DanmuDisplay* danmuDisplay;
};

#endif