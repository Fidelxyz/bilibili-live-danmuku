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
    LiveRoom(QWidget* parent);
    ~LiveRoom();
    bool isStarted();

   private:
    QJsonObject requestDanmuInfo();
    int requestUid();

   signals:
    void updateFollowersCount(const int& followersCount);

   public slots:
    void slotStart();
    void slotUpdateFollowersCount();

   public:
    Protocal* protocal;

   private:
    QLineEdit* input_roomID;
    QPushButton* btn_start;

    bool started;

    int roomID;
    int uid;
    QThread protocalThread;
    QTimer* updateFollowersCountTimer;
};

#endif