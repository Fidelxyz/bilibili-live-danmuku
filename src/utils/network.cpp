#include "network.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QJsonObject requestJsonResponse(QString url) {
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(url);

    QEventLoop eventLoop;
    QNetworkReply *reply = networkAccessManager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray responseByte = reply->readAll();
    qDebug() << "getJsonResponse:" << responseByte;

    return QJsonDocument::fromJson(responseByte).object();
}