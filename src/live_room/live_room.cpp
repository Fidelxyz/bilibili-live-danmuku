#include "live_room.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QString>

#include "danmu.h"

LiveRoom::LiveRoom(const int roomID) {
    this->roomID = roomID;
    Danmu *danmu = new Danmu(roomID, getInfo());
}

LiveRoom::~LiveRoom() {}

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

    QJsonObject jsonObject = QJsonDocument::fromJson(responseByte).object();
    return jsonObject;
}