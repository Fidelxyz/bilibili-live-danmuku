#include "live_room.h"

#include <QJsonObject>

#include "utils/network.h"

LiveRoom::LiveRoom() : Module("live_room") {
    protocal = nullptr;

    // Control UI
    QHBoxLayout *layout = new QHBoxLayout(widget);

    input_roomID = new QLineEdit(widget);
    input_roomID->setText("14003442");  // debug
    layout->addWidget(input_roomID);

    btn_start = new QPushButton("连接", widget);
    connect(btn_start, SIGNAL(clicked()), this, SLOT(start()));
    layout->addWidget(btn_start);
}

LiveRoom::~LiveRoom() {
    qDebug("Enter ~LiveRoom");
    stop();
    qDebug("Exit ~LiveRoom");
}

void LiveRoom::start() {
    if (protocal != nullptr) {
        stop();
    }

    roomID = input_roomID->text().toInt();
    protocal = new Protocal();
    protocal->moveToThread(&protocalThread);
    protocalThread.start();
    QMetaObject::invokeMethod(protocal, "startConnection",
                              Q_ARG(const int &, roomID),
                              Q_ARG(const QJsonObject &, requestDanmuInfo()));

    uid = requestUid();
}

bool LiveRoom::isRunning() {
    qDebug() << "isRunning:" << (protocal != nullptr);
    return protocal != nullptr;
}

void LiveRoom::stop() {
    qDebug("Enter stop");
    if (protocal == nullptr) {
        qDebug("Exit stop: Protocal have not started.");
        return;
    }
    QMetaObject::invokeMethod(protocal, "stopConnection",
                              (Qt::ConnectionType)Qt::BlockingQueuedConnection);
    protocalThread.quit();
    protocalThread.wait();
    protocal->deleteLater();
    qDebug("Exit stop");
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

void LiveRoom::updateFollowersCount() {
    QJsonObject response = requestJsonResponse(
        QString("https://api.bilibili.com/x/relation/stat?vmid=%1&jsonp=jsonp")
            .arg(uid));
    if (response["code"].toInt() != 0) {
        qWarning("Error occured in followersCountUpdated.");
        return;
    }
    qDebug() << "followersCountUpdated:"
             << response["data"][(QString) "follower"].toInt();
    emit followersCountUpdated(response["data"][(QString) "follower"].toInt());
}