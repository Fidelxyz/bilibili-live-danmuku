#include "live_room.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

LiveRoom::LiveRoom(const int roomID) {
    this->roomID = roomID;
    protocal = new Protocal();
    protocal->moveToThread(&protocalThread);
    connect(
        this, SIGNAL(startProtocalConnection(const int &, const QJsonObject &)),
        protocal, SLOT(slotStartConnection(const int &, const QJsonObject &)));
    protocalThread.start();
    emit startProtocalConnection(roomID, getInfo());
    initDisplay();
}

LiveRoom::~LiveRoom() {
    delete protocal;
    protocalThread.quit();
    delete danmuDisplay;
}

QJsonObject LiveRoom::getInfo() {
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    QString url = QString(
                      "https://api.live.bilibili.com/xlive/web-room/v1/index/"
                      "getDanmuInfo?type=0&;id=%1")
                      .arg(roomID);
    QNetworkRequest request;
    request.setUrl(url);

    QEventLoop eventLoop;
    QNetworkReply *reply = networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray responseByte = reply->readAll();
    qDebug() << responseByte;

    QJsonObject jsonObj = QJsonDocument::fromJson(responseByte).object();
    return jsonObj;
}

void LiveRoom::initDisplay() {
    danmuDisplay = new DanmuDisplay();
    connect(protocal,
            SIGNAL(recvDanmu(const int &, const QString &, const QString &,
                             const bool &, const bool &, const int &)),
            danmuDisplay,
            SLOT(slotRecvDanmu(const int &, const QString &, const QString &,
                               const bool &, const bool &, const int &)));
    connect(protocal, SIGNAL(updateViewersCount(const int &)), danmuDisplay,
            SLOT(slotUpdateViewersCount(const int &)));
    danmuDisplay->show();
}