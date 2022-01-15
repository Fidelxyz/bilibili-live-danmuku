#include "live_room.h"

#include <QJsonObject>

#include "utils/network.h"

LiveRoom::LiveRoom() : Module("live_room") {
    protocal = nullptr;

    // Control UI
    QHBoxLayout *layout = new QHBoxLayout(widget);  // deleted by QT

    input_roomID = new QLineEdit(widget);  // deleted by QT
#ifdef QT_DEBUG
    input_roomID->setText("14003442");  // debug
#endif
    layout->addWidget(input_roomID);

    btn_start = new QPushButton(tr("连接"), widget);  // deleted by QT
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

    bool ok;
    roomID = input_roomID->text().toInt(&ok);
    if (!ok) {
        qWarning("Invalid room ID. Exit.");
        return;
    }

    QJsonObject danmuInfo = requestDanmuInfo();
    if (danmuInfo["code"].toInt() != 0) {
        qWarning("Danmu info is not vaild. Exit.");
        return;
    }

    Q_ASSERT(protocal == nullptr);
    protocal = new Protocal();  // deleted in stop()
    protocal->moveToThread(&protocalThread);
    protocalThread.start();

    QMetaObject::invokeMethod(protocal, "startConnection",
                              Q_ARG(const int &, roomID),
                              Q_ARG(const QJsonObject &, danmuInfo));

    uid = requestUid();

    emit started();
}

void LiveRoom::stop() {
    qDebug("Enter stop");
    if (protocal == nullptr) {
        qDebug("Exit stop: Protocal has not been started.");
        return;
    }
    emit stopped();
    QMetaObject::invokeMethod(protocal, "stopConnection",
                              (Qt::ConnectionType)Qt::BlockingQueuedConnection);
    protocalThread.quit();
    protocalThread.wait();
    // protocal->deleteLater();
    delete protocal;
    protocal = nullptr;
    qDebug("Exit stop");
}

QObject *LiveRoom::getProtocal() {
    return protocal;
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