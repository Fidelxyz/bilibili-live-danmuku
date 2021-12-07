#ifndef DANMU_H
#define DANMU_H

#include <QObject>
#include <QWebSocket>

#include "network.h"

class Danmu : public QObject {
    Q_OBJECT
   public:
    Danmu(const int roomID, const QJsonObject &liveRoomInfo);
    ~Danmu();

   private slots:
    void slotRecvBinaryMsg(QByteArray msg);

   public:
    QWebSocket *ws;
};

#endif