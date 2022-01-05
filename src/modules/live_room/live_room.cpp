#include "live_room.h"

#include <QJsonObject>

#include "utils/network.h"

const ModuleMetadata LiveRoom::moduleMetadata = {"live_room", {}};

LiveRoom::LiveRoom(QWidget *parent) : Module(parent) {
    started = false;

    // Control UI
    QHBoxLayout *layout = new QHBoxLayout(widget);

    input_roomID = new QLineEdit(widget);
    input_roomID->setText("14003442");  // debug
    layout->addWidget(input_roomID);

    btn_start = new QPushButton("连接", widget);
    connect(btn_start, SIGNAL(clicked()), this, SLOT(slotStart()));
    layout->addWidget(btn_start);
}

LiveRoom::~LiveRoom() {
    QMetaObject::invokeMethod(protocal, "slotStopConnection",
                              (Qt::ConnectionType)Qt::BlockingQueuedConnection);
    protocalThread.quit();
    protocalThread.wait();
    delete protocal;

    delete input_roomID;
    delete btn_start;
}

bool LiveRoom::isStarted() { return started; }

void LiveRoom::slotStart() {
    started = true;

    roomID = input_roomID->text().toInt();
    protocal = new Protocal();
    protocal->moveToThread(&protocalThread);
    protocalThread.start();
    QMetaObject::invokeMethod(protocal, "slotStartConnection",
                              Q_ARG(const int &, roomID),
                              Q_ARG(const QJsonObject &, requestDanmuInfo()));

    uid = requestUid();

    slotUpdateFollowersCount();
    updateFollowersCountTimer = new QTimer(this);
    connect(updateFollowersCountTimer, SIGNAL(timeout()), this,
            SLOT(slotUpdateFollowersCount()));
    updateFollowersCountTimer->start(30000);  // 30s
}

QJsonObject LiveRoom::requestDanmuInfo() {
    QJsonObject response = requestJsonResponse(
        QString("https://api.live.bilibili.com/xlive/web-room/v1/index/"
                "getDanmuInfo?id=%1")
            .arg(roomID));
    if (response["code"].toInt() != 0) {
        qWarning("Error occured in requestDanmuInfo.");
    }
    return response;
}

int LiveRoom::requestUid() {
    QJsonObject response = requestJsonResponse(
        QString("https://api.live.bilibili.com/xlive/web-room/v2/index/"
                "getRoomPlayInfo?room_id=%1&protocol=0,1&format=0,1,2&"
                "codec=0,"
                "1&qn=0&platform=web&ptype=8")
            .arg(roomID));
    if (response["code"].toInt() != 0) {
        qWarning("Error occured in requestUid.");
        return 0;
    }
    return response["data"].toObject()["uid"].toInt();
}

void LiveRoom::slotUpdateFollowersCount() {
    QJsonObject response = requestJsonResponse(
        QString("https://api.bilibili.com/x/relation/stat?vmid=%1&jsonp=jsonp")
            .arg(uid));
    if (response["code"].toInt() != 0) {
        qWarning("Error occured in slotUpdateFollowersCount.");
        return;
    }
    qDebug() << "slotUpdateFollowersCount:"
             << response["data"][(QString) "follower"].toInt();
    emit updateFollowersCount(response["data"][(QString) "follower"].toInt());
}