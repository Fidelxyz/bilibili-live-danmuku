#ifndef LIVE_ROOM_H
#define LIVE_ROOM_H

#include <QJsonObject>
#include <QObject>

class LiveRoom : public QObject {
    Q_OBJECT

   public:
    LiveRoom(const int roomID);
    ~LiveRoom();

    QJsonObject getInfo();

    int roomID;
};

#endif