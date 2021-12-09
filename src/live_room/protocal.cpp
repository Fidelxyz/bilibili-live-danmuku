#include "protocal.h"

#include <QByteArray>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QTimer>

#include "utils/byte_convert.h"
#include "utils/decompress.h"

#define WS_OP_USER_AUTHENTICATION 7
#define WS_HEADER_DEFAULT_SEQUENCE 1
#define WS_PACKAGE_HEADER_TOTAL_LENGTH 16
#define WS_HEADER_DEFAULT_VERSION 1
#define WS_OP_CONNECT_SUCCESS 8
#define WS_OP_HEARTBEAT 2
// Header
#define WS_PACKAGE_OFFSET 0
#define WS_HEADER_OFFSET 4
#define WS_VERSION_OFFSET 6
#define WS_OPERATION_OFFSET 8
#define WS_SEQUENCE_OFFSET 12

QByteArray genHead(int datalength, int opeation, int sequence) {
    QByteArray buffer = QByteArray();
    buffer.append(
        int32ToBytes((uint32_t)(datalength + WS_PACKAGE_HEADER_TOTAL_LENGTH)));
    buffer.append(int16ToBytes((uint16_t)(WS_PACKAGE_HEADER_TOTAL_LENGTH)));
    buffer.append(int16ToBytes((uint16_t)(WS_HEADER_DEFAULT_VERSION)));
    buffer.append(int32ToBytes((uint32_t)(opeation)));
    buffer.append(int32ToBytes((uint32_t)(sequence)));
    return buffer;
}

bool checkHello(QByteArray data) {
    QByteArray valData("{\"code\":0}");
    valData.prepend(genHead(valData.length(), WS_OP_CONNECT_SUCCESS,
                            WS_HEADER_DEFAULT_SEQUENCE));
    return valData == data;
}

Protocal::Protocal(const int roomID, const QJsonObject &liveRoomInfo) {
    QJsonObject liveRoomInfoData = liveRoomInfo["data"].toObject();
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

    data.prepend(genHead(data.length(), WS_OP_USER_AUTHENTICATION,
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
            SLOT(slotRecvData(QByteArray)));

    heartBeatTimer = new QTimer(this);
    connect(heartBeatTimer, SIGNAL(timeout()), this, SLOT(slotSendHeartbeat()));
    heartBeatTimer->start(30000);  // 30s
}

Protocal::~Protocal() {
    delete heartBeatTimer;
    ws->close();
    delete ws;
}

void Protocal::slotRecvData(QByteArray data) {
    qDebug() << "-------------------------------";
    qDebug() << "receive message: " << data;

    if (data.length() <= WS_PACKAGE_HEADER_TOTAL_LENGTH) {
        qWarning() << "Package is broken.";
        return;
    }

    QByteArray head = data.left(WS_PACKAGE_HEADER_TOTAL_LENGTH);

    uint32_t packLen = bytesToInt32(head.mid(WS_PACKAGE_OFFSET, 4));
    uint16_t headLen = bytesToInt16(head.mid(WS_HEADER_OFFSET, 2));
    uint16_t ver = bytesToInt16(head.mid(WS_VERSION_OFFSET, 2));
    uint32_t type = bytesToInt32(head.mid(WS_OPERATION_OFFSET, 4));
    // uint32_t seque = bytesToInt32(head.mid(WS_SEQUENCE_OFFSET, 4));

    qDebug() << "ver =" << ver << ", type =" << type;
    qDebug() << "datalength =" << data.length() << ", packLen =" << packLen;

    if (data.length() < packLen) {
        qWarning() << "Package is broken.";
        return;
    }

    QByteArray pack;
    switch (ver) {
        case DEFLATE:
            if (!decompressDeflate(data.mid(headLen), pack)) {
                qWarning() << "Decompression (deflate) failed.";
                return;
            }
            break;
        case BROTLI:
            if (!decompressBrotli(data.mid(headLen), pack)) {
                qWarning() << "Decompression (brotli) failed.";
                return;
            }
            break;
    }

    while (pack.length() != 0) {
        qDebug() << "pack: " << pack;

        head = pack.left(WS_PACKAGE_HEADER_TOTAL_LENGTH);

        int packLen = bytesToInt32(head.mid(WS_PACKAGE_OFFSET, 4));
        int headLen = bytesToInt16(head.mid(WS_HEADER_OFFSET, 2));
        // int ver = bytesToInt16(head.mid(WS_VERSION_OFFSET, 2));
        int type = bytesToInt32(head.mid(WS_OPERATION_OFFSET, 4));
        // int seque = bytesToInt32(head.mid(WS_SEQUENCE_OFFSET, 4));

        if (pack.length() < packLen) {
            qWarning() << "Package is broken.";
        }

        int payloadLen = packLen - headLen;

        QByteArray msg = pack.left(packLen).mid(WS_PACKAGE_HEADER_TOTAL_LENGTH);
        switch (type) {
            case MESSAGE:
                qDebug() << "Message received: " << msg;
                recvMsg(QJsonDocument::fromJson(msg).object());
                break;
            case HEARTBEAT_REPLY:
                qDebug() << "Heartbeat received: " << msg;
                recvHeartbeatReply(msg);
                break;
            default:
                qWarning() << "Unknown message, type = " << type << ".";
                return;
        }

        pack = pack.mid(packLen);
    }
}

void Protocal::slotSendHeartbeat() {
    QByteArray data("[object Object]");
    data.prepend(
        genHead(data.length(), WS_OP_HEARTBEAT, WS_HEADER_DEFAULT_SEQUENCE));
    ws->sendBinaryMessage(data);
}

void Protocal::recvHeartbeatReply(const QByteArray &msg) {
    emit updateViewersCount(bytesToInt32(msg.left(4)));
}

void Protocal::recvMsg(const QJsonObject &msg) {
    QString cmd = msg["cmd"].toString();
    // TODO
}