#ifndef DANMU_H
#define DANMU_H

#include <QTimer>
#include <QWebSocket>

QByteArray genHead(int datalength, int opeation, int sequence);

class Protocal : public QObject {
    Q_OBJECT

   public:
    Protocal();

   private:
    void recvHeartbeatReply(const QByteArray &msg);
    void recvMsg(const QJsonObject &msg);

   signals:
    void updateViewersCount(const int &viewersCount);
    void recvDanmu(const int &uid, const QString &username, const QString &text,
                   const bool &isAdmin, const bool &isVIP,
                   const int &userGuardLevel);

   public slots:
    void slotStartConnection(const int &roomID,
                             const QJsonObject &liveRoomInfo);
    void slotStopConnection();

   private slots:
    void slotSendHeartbeat();
    void slotRecvData(const QByteArray &data);

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