#ifndef NETWORK_H
#define NETWORK_H

#include <QAbstractSocket>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QWebSocket>

namespace Network {
QJsonObject getLiveRoomInfo(int roomID);
}

class WebSocketClient : public QObject {
    Q_OBJECT

   public:
    explicit WebSocketClient(QObject *parent = 0);
    ~WebSocketClient();

    void connectReq(const QNetworkRequest &request);  // 连接websocket服务器的URL
    void close();                                  // 关闭websocket
    void sendTextMsg(const QString &message);      // 发送Text类型的消息
    void sendBinaryMsg(const QByteArray &data);  // 发送Binary类型的消息
    bool getConStatus();                         // 返回服务器连接状态

   signals:
    void sigRecvTextMsg(QString message);  // 接受到Text类型消息的信号

   private slots:
    void slotConnected();                        // 连接成功
    void slotDisconnected();                     // 断开连接
    void slotRecvTextMsg(QString message);       // 接受字符数据
    void slotRecvBinaryMsg(QByteArray message);  // 接受二进制数据
    void slotError(QAbstractSocket::SocketError error);  // 响应报错

   private:
    void reconnect();  // 断开重连

   public:
    QWebSocket *m_pWebSocket;

   private:
    QUrl m_url;
    bool m_bConnected = false;  // 为true,表明已连接服务器，否则未连接上
};

#endif