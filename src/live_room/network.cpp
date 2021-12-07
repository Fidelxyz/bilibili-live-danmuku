#include "network.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QString>
#include <QWebSocket>



WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent) {
    m_pWebSocket = new QWebSocket();

    // 连接相应的信号槽
    connect(m_pWebSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(m_pWebSocket, SIGNAL(disconnected()), this,
            SLOT(slotDisconnected()));
    connect(m_pWebSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(slotError(QAbstractSocket::SocketError)));
}

WebSocketClient::~WebSocketClient() {
    if (m_pWebSocket != 0) {
        m_pWebSocket->deleteLater();
        m_pWebSocket = 0;
    }
}

// 连接websocket服务器的URL
void WebSocketClient::connectReq(const QNetworkRequest &request) {
    m_pWebSocket->open(request);
}

// 关闭websocket
void WebSocketClient::close() { m_pWebSocket->close(); }

// 发送Text类型的消息
void WebSocketClient::sendTextMsg(const QString &message) {
    if (!m_bConnected) {
        qDebug() << __FILE__ << __LINE__ << "Failed to" << __FUNCTION__
                 << ", it's not running...";
        return;
    }
    qDebug() << "send: " << message;
    m_pWebSocket->sendTextMessage(message);
}

// 发送Binary类型的消息
void WebSocketClient::sendBinaryMsg(const QByteArray &data) {
    if (!m_bConnected) {
        qDebug() << __FILE__ << __LINE__ << "Failed to" << __FUNCTION__
                 << ", it's not running...";
        return;
    }
    qDebug() << "send: " << data;
    m_pWebSocket->sendBinaryMessage(data);
    qDebug() << "send finished";
}

// 返回服务器连接状态
bool WebSocketClient::getConStatus() { return m_bConnected; }

// 连接成功
void WebSocketClient::slotConnected() {
    qDebug() << "connect successful";
    m_bConnected = true;

    connect(m_pWebSocket, SIGNAL(textMessageReceived(QString)), this,
            SLOT(slotRecvTextMsg(QString)));
    connect(m_pWebSocket, SIGNAL(binaryMessageReceived(QByteArray)), this,
            SLOT(slotRecvBinaryMsg(QByteArray)));
}

// 断开连接
void WebSocketClient::slotDisconnected() {
    qDebug() << __FILE__ << __LINE__ << "disconnected";
    reconnect();
}

// 接受字符数据
void WebSocketClient::slotRecvTextMsg(QString message) {
    emit sigRecvTextMsg(message);
}

// 接受二进制数据
void WebSocketClient::slotRecvBinaryMsg(QByteArray message) {
    qDebug() << "slotRecvBinaryMsg: " << message;
}

// 响应报错
void WebSocketClient::slotError(QAbstractSocket::SocketError error) {
    qDebug() << __FILE__ << __LINE__ << (int)error
             << m_pWebSocket->errorString();
}

// 断开重连
void WebSocketClient::reconnect() {
    qDebug() << "websocket reconnected";
    m_pWebSocket->abort();
    m_pWebSocket->open(m_url);
}