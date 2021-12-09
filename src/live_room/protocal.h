#ifndef DANMU_H
#define DANMU_H

#include <QByteArray>
#include <QJsonObject>
#include <QObject>
#include <QTimer>
#include <QWebSocket>

QByteArray genHead(int datalength, int opeation, int sequence);

class Protocal : public QObject {
    Q_OBJECT
   public:
    Protocal(const int roomID, const QJsonObject &liveRoomInfo);
    ~Protocal();

   signals:
    void updateViewersCount(int viewersCount);

   private:
    void recvHeartbeatReply(const QByteArray &msg);
    void recvMsg(const QJsonObject &msg);
    enum BODY_PROTOCAL_VERSION { DEFLATE = 2, BROTLI = 3 };
    enum MSG_TYPE { HEARTBEAT_REPLY = 3, MESSAGE = 5, AUTH_REPLY = 8 };

   private slots:
    void slotSendHeartbeat();
    void slotRecvData(QByteArray data);

   private:
    QWebSocket *ws;
    QTimer *heartBeatTimer;
};

#endif