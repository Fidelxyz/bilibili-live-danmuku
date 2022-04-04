#ifndef DANMU_H
#define DANMU_H

#include <QTimer>
#include <QWebSocket>

const int WS_OP_USER_AUTHENTICATION = 7;
const int WS_HEADER_DEFAULT_SEQUENCE = 1;
const int WS_PACKAGE_HEADER_TOTAL_LENGTH = 16;
const int WS_HEADER_DEFAULT_VERSION = 1;
const int WS_OP_CONNECT_SUCCESS = 8;
const int WS_OP_HEARTBEAT = 2;
// Header
const int WS_PACKAGE_OFFSET = 0;
const int WS_HEADER_OFFSET = 4;
const int WS_VERSION_OFFSET = 6;
const int WS_OPERATION_OFFSET = 8;
const int WS_SEQUENCE_OFFSET = 12;

const int WS_HEARTBEAT_INTERVAL_MS = 30000;

QByteArray genHead(int datalength, int opeation, int sequence);

class Protocal : public QObject {
    Q_OBJECT

   public:
    Protocal();

   private:
    void recvHeartbeatReply(const QByteArray &msg);
    void recvMsg(const QJsonObject &msg);
    void sendHeartbeat();
    void recvData(const QByteArray &data);

   signals:
    void viewersCountUpdated(const int &viewersCount);
    void recvDanmu(const int uid, const QString &username, const QString &text,
                   const bool isAdmin, const bool isVIP,
                   const int userGuardLevel);
    void recvGift(const int uid, const QString &username,
                  const QString &giftName, const int giftCount);

   public slots:
    void startConnection(const int &roomID, const QJsonObject &liveRoomInfo);
    void stopConnection();

   private:
    QWebSocket *ws;
    QTimer *heartBeatTimer;

    enum BODY_PROTOCAL_VERSION { DEFLATE = 2, BROTLI = 3 };
    enum MSG_TYPE { HEARTBEAT_REPLY = 3, MESSAGE = 5, AUTH_REPLY = 8 };
    enum CMD {
        UNKNOWN = 0,
        LIVE,
        PREPARING,
        DANMU_MSG,
        SEND_GIFT,
        COMBO_SEND,
        GIFT_TOP,
        WELCOME,
        WELCOME_GUARD,
        ENTRY_EFFECT,
        GUARD_BUY,
        SUPER_CHAT_MESSAGE,
        SUPER_CHAT_MESSAGE_JP,
        INTERACT_WORD,
        WARNING,
        CUT_OFF
    };

    static const QHash<QString, enum CMD> cmdMap;
};

#endif