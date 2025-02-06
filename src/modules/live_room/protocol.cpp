#include "protocol.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>

#include "utils/byte_convert.h"
#include "utils/decompress.h"

#define CMD_MAP(CMD) {QString(#CMD), CMD}
// clang-format off
const QHash<QString, Protocol::CMD> Protocol::cmdMap = QHash<QString, CMD>({
    CMD_MAP(LIVE),
    CMD_MAP(PREPARING),
    CMD_MAP(DANMU_MSG),
    CMD_MAP(SEND_GIFT),
    CMD_MAP(COMBO_SEND),
    CMD_MAP(GIFT_TOP),
    CMD_MAP(WELCOME),
    CMD_MAP(WELCOME_GUARD),
    CMD_MAP(ENTRY_EFFECT),
    CMD_MAP(GUARD_BUY),
    CMD_MAP(SUPER_CHAT_MESSAGE),
    CMD_MAP(SUPER_CHAT_MESSAGE_JP),
    CMD_MAP(INTERACT_WORD),
    CMD_MAP(WARNING),
    CMD_MAP(CUT_OFF)
});
// clang-format on
#undef CMD_MAP

QByteArray Protocol::genHead(const uint32_t datalength, const uint32_t opeation,
                             const uint32_t sequence) {
    auto buffer = QByteArray();
    buffer.append(int32ToBytes(datalength + WS_PACKAGE_HEADER_TOTAL_LENGTH));
    buffer.append(int16ToBytes(WS_PACKAGE_HEADER_TOTAL_LENGTH));
    buffer.append(int16ToBytes(WS_HEADER_DEFAULT_VERSION));
    buffer.append(int32ToBytes(opeation));
    buffer.append(int32ToBytes(sequence));
    return buffer;
}

void Protocol::startConnection(const int&         roomID,
                               const QJsonObject& liveRoomInfo) {
    QJsonObject liveRoomInfoData = liveRoomInfo["data"].toObject();
    QString     url   = liveRoomInfoData["host_list"][0]["host"].toString();
    QString     token = liveRoomInfoData["token"].toString();

    url.prepend("wss://").append("/sub");

    const QJsonObject subInfo{
        {"uid",      0     },
        {"roomid",   roomID},
        {"protover", 3     },
        {"platform", "web" },
        {"type",     "2"   },
        {"key",      token }
    };

    QByteArray data = QJsonDocument(subInfo).toJson(QJsonDocument::Compact);

    data.prepend(genHead(static_cast<int>(data.length()),
                         WS_OP_USER_AUTHENTICATION,
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

    ws = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest,
                        this);  // deleted in stopConnection()
    ws->open(request);

    QEventLoop eventLoop;
    connect(ws, &QWebSocket::connected, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    qDebug("WS connected");
    ws->sendBinaryMessage(data);
    qDebug() << "send: " << data;

    connect(ws, &QWebSocket::binaryMessageReceived, this, &Protocol::recvData,
            Qt::QueuedConnection);

    heartBeatTimer = new QTimer(this);  // deleted in stopConnection()
    connect(heartBeatTimer, &QTimer::timeout, this, &Protocol::sendHeartbeat);
    heartBeatTimer->start(WS_HEARTBEAT_INTERVAL_MS);
}

void Protocol::stopConnection() const {
    qDebug("Enter stopConnection");
    Q_ASSERT(heartBeatTimer != nullptr);
    heartBeatTimer->stop();
    heartBeatTimer->deleteLater();
    Q_ASSERT(ws != nullptr);
    ws->close();
    ws->deleteLater();
    qDebug("Exit stopConnection");
}

QByteArray Protocol::decompressPack(const QByteArray& data) {
    const QByteArray head    = data.left(WS_PACKAGE_HEADER_TOTAL_LENGTH);
    const uint32_t   packLen = bytesToInt32(head.mid(WS_PACKAGE_OFFSET, 4));
    const uint16_t   headLen = bytesToInt16(head.mid(WS_HEADER_OFFSET, 2));
    const uint16_t   ver     = bytesToInt16(head.mid(WS_VERSION_OFFSET, 2));
    const uint32_t   type    = bytesToInt32(head.mid(WS_OPERATION_OFFSET, 4));
    // const uint32_t seque = bytesToInt32(head.mid(WS_SEQUENCE_OFFSET, 4));

    qDebug() << "ver =" << ver << ", type =" << type;
    qDebug() << "datalength =" << data.length() << ", packLen =" << packLen;

    if (data.length() < packLen) {
        qWarning("Package is broken.");
        return {};
    }

    QByteArray pack;
    switch (ver) {
        case DEFLATE:
            Q_ASSERT(type == 5);
            // if (!decompressDeflate(data.mid(headLen), pack)) {
            //     qWarning("Decompression (deflate) failed.");
            //     return;
            // }
            break;
        case BROTLI:
            Q_ASSERT(type == 5);
            if (!decompressBrotli(data.mid(headLen), &pack)) {
                qWarning("Decompression (brotli) failed.");
                return {};
            }
            break;
        default:
            pack = data;
            break;
    }

    return pack;
}

void Protocol::recvData(const QByteArray& data) {
    qDebug() << "-------------------------------";
    qDebug() << "receive message: " << data;

    if (data.length() <= WS_PACKAGE_HEADER_TOTAL_LENGTH) {
        qWarning("Package is broken.");
        return;
    }

    // Decompress pack
    QByteArray pack = decompressPack(data);
    if (pack.isEmpty()) return;

    while (pack.length() != 0) {
        qDebug() << "pack:" << pack;

        if (pack == "[object Object]") {  // heartbeat reply
            break;
        }

        const QByteArray head    = pack.left(WS_PACKAGE_HEADER_TOTAL_LENGTH);
        const uint32_t   packLen = bytesToInt32(head.mid(WS_PACKAGE_OFFSET, 4));
        // const uint32_t headLen = bytesToInt16(head.mid(WS_HEADER_OFFSET, 2));
        // const uint32_t ver = bytesToInt16(head.mid(WS_VERSION_OFFSET, 2));
        const uint32_t type = bytesToInt32(head.mid(WS_OPERATION_OFFSET, 4));
        // const uint32_t seque = bytesToInt32(head.mid(WS_SEQUENCE_OFFSET, 4));

        if (pack.length() < packLen) {
            qWarning("Package is broken.");
        }

        // int payloadLen = packLen - headLen;

        QByteArray msg = pack.left(packLen).mid(WS_PACKAGE_HEADER_TOTAL_LENGTH);
        switch (type) {
            case MESSAGE:
                qDebug() << "Message received:" << msg;
                recvMsg(QJsonDocument::fromJson(msg).object());
                break;
            case HEARTBEAT_REPLY:
                qDebug() << "Heartbeat reply received:" << msg;
                recvHeartbeatReply(msg);
                break;
            case AUTH_REPLY:
                qDebug() << "Auth reply received:" << msg;
                break;
            default:
                qWarning() << "Unknown message, type =" << type << ".";
                return;
        }

        pack = pack.mid(packLen);
    }
}

void Protocol::sendHeartbeat() const {
    qDebug("sendHeartbeat");
    QByteArray data("[object Object]");
    data.prepend(
        genHead(data.length(), WS_OP_HEARTBEAT, WS_HEADER_DEFAULT_SEQUENCE));
    ws->sendBinaryMessage(data);
}

void Protocol::recvHeartbeatReply(const QByteArray& msg) {
    qDebug() << "Update viewers count (protocol):" << bytesToInt32(msg.left(4));
    emit viewersCountUpdated(static_cast<int>(bytesToInt32(msg.left(4))));
}

void Protocol::recvMsg(const QJsonObject& msg) {
    switch (cmdMap[msg["cmd"].toString()]) {
        case DANMU_MSG: {
            const QJsonValue info           = msg["info"];
            const QString    text           = info[1].toString();
            const int        uid            = info[2][0].toInt();
            const QString    username       = info[2][1].toString();
            const bool       isAdmin        = info[2][2].toString() == "1";
            const bool       isVIP          = info[2][3].toString() == "1";
            const int        userGuardLevel = info[7].toInt();

            emit recvDanmu(uid, username, text, isAdmin, isVIP, userGuardLevel);
            qDebug() << "Emit recvDanmu:" << uid << username << text << isAdmin
                     << isVIP << userGuardLevel;
            break;
        }
        case SEND_GIFT: {
            const QJsonValue data      = msg["data"];
            const int        uid       = data["uid"].toInt();
            const QString    username  = data["uname"].toString();
            const QString    giftName  = data["giftName"].toString();
            const int        giftCount = data["num"].toInt();

            emit recvGift(uid, username, giftName, giftCount);
            qDebug() << "Emit recvGift:" << uid << username << giftName
                     << giftCount;
            break;
        }
        default:
            break;
    }
}
