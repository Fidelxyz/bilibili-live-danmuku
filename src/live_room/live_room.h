#ifndef LIVE_ROOM_H
#define LIVE_ROOM_H

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

   private:
    int roomID;

    Protocal* protocal;
    DanmuDisplay* display;
};

#endif