#include "live_room.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

LiveRoom::LiveRoom(const int roomID) {
    this->roomID = roomID;
    protocal = new Protocal(roomID, getInfo());
    initDisplay();
}

LiveRoom::~LiveRoom() {
    delete protocal;
    delete display;
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
    display = new DanmuDisplay();
    connect(protocal,
            SIGNAL(recvDanmu(const int &, const QString &, const QString &,
                             const bool &, const bool &, const int &)),
            display,
            SLOT(slotRecvDanmu(const int &, const QString &, const QString &,
                               const bool &, const bool &, const int &)));
    connect(protocal, SIGNAL(updateViewersCount(const int &)), display,
            SLOT(slotUpdateViewersCount(const int &)));
    display->show();
}