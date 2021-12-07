#include "danmu.h"

#include <QByteArray>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "utils/byte_convert.h"

// 第一次订阅需要
#define WS_OP_USER_AUTHENTICATION 7
#define WS_HEADER_DEFAULT_SEQUENCE 1
#define WS_PACKAGE_HEADER_TOTAL_LENGTH 16
#define WS_HEADER_DEFAULT_VERSION 1
// 第一次订阅返回需要
#define WS_OP_CONNECT_SUCCESS 8
#define WS_HEADER_DEFAULT_SEQUENCE 1
// 心跳需要
#define WS_OP_HEARTBEAT 2
#define WS_HEADER_DEFAULT_SEQUENCE 1
// 解析返回信息
#define WS_PACKAGE_OFFSET 0
#define WS_HEADER_OFFSET 4
#define WS_VERSION_OFFSET 6
#define WS_OPERATION_OFFSET 8
#define WS_SEQUENCE_OFFSET 12
#define WS_BODY_PROTOCOL_VERSION_DEFLATE 2
#define WS_OP_MESSAGE 5
#define WS_OP_HEARTBEAT_REply 3

QByteArray headGen(int datalength, int opeation, int sequence) {
    QByteArray buffer = QByteArray();
    buffer.append(
        int32ToBytes((uint32_t)(datalength + WS_PACKAGE_HEADER_TOTAL_LENGTH)));
    buffer.append(int16ToBytes((uint16_t)(WS_PACKAGE_HEADER_TOTAL_LENGTH)));
    buffer.append(int16ToBytes((uint16_t)(WS_HEADER_DEFAULT_VERSION)));
    buffer.append(int32ToBytes((uint32_t)(opeation)));
    buffer.append(int32ToBytes((uint32_t)(sequence)));
    return buffer;
}

Danmu::Danmu(const int roomID, const QJsonObject &liveRoomInfo) {
    QJsonObject liveRoomInfoData =
        liveRoomInfo.value(QStringLiteral("data")).toObject();
    QString url = liveRoomInfoData["host_list"]
                      .toArray()[0]
                      .toObject()["host"]
                      .toString();
    QString token = liveRoomInfoData["token"].toString();

    url.prepend("wss://").append("/sub");

    QJsonObject subInfo{{"uid", 0},      {"roomid", roomID},
                        {"protover", 3}, {"platform", "web"},
                        {"type", "2"},   {"key", token}};

    QByteArray data = QJsonDocument(subInfo).toJson(QJsonDocument::Compact);

    data.prepend(headGen(data.length(), WS_OP_USER_AUTHENTICATION,
                         WS_HEADER_DEFAULT_SEQUENCE));

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; "
                      "x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                      "Chrome/96.0.4664.55 Safari/537.36 Edg/96.0.1054.43");
    request.setRawHeader("Origin", "https://live.bilibili.com");
    request.setRawHeader("Cache-Control", "no-cache");
    request.setRawHeader("Pragma", "no-cache");
    request.setRawHeader("Accept-Language",
                         "zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6");

    ws = new QWebSocket();
    ws->open(request);

    QEventLoop eventLoop;
    QObject::connect(ws, SIGNAL(connected()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    qDebug() << "WS connected";
    ws->sendBinaryMessage(data);
    qDebug() << "send: " << data;

    connect(ws, SIGNAL(binaryMessageReceived(QByteArray)), this,
            SLOT(slotRecvBinaryMsg(QByteArray)));
}

Danmu::~Danmu() { ws->close(); }

Danmu::slotRecvBinaryMsg(QByteArray data) {
    qDebug() << "receive data: " << data;
    
}
