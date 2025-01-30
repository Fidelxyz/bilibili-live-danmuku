#ifndef DANMUKU_MODULES_LIVE_ROOM_LIVE_ROOM_H_
#define DANMUKU_MODULES_LIVE_ROOM_LIVE_ROOM_H_

#include <QHBoxLayout>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QThread>

#include "module.h"
#include "protocol.h"

class LiveRoom : public Module {
    Q_OBJECT
    MODULE

   public:
    LiveRoom(Danmuku* parent);
    ~LiveRoom();

   private:
    QJsonObject requestDanmuInfo();
    int requestUid();
    void stop();

   signals:
    void started();
    void stopped();
    void followersCountUpdated(const int& followersCount);

   public slots:
    void start();
    QObject* getProtocol();
    void updateFollowersCount();

   private:
    Protocol* protocol;
    QLineEdit* input_roomID;
    QPushButton* btn_start;

    int roomID;
    int uid;
    QThread protocolThread;
};

#endif  // DANMUKU_MODULES_LIVE_ROOM_LIVE_ROOM_H_