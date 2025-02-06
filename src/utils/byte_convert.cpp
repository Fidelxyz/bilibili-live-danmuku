#include "byte_convert.h"

#include <QByteArray>
#include <QtEndian>
#include <cstdint>

QByteArray int16ToBytes(const uint16_t data) {
    if constexpr (Q_BYTE_ORDER == Q_LITTLE_ENDIAN) {
        return QByteArray::fromRawData(reinterpret_cast<const char *>(&data),
                                       sizeof(data));
    } else {
        QByteArray bytes(sizeof(data), 0);
        qToLittleEndian<quint16>(data, bytes.data());
        return bytes;
    }
}

QByteArray int32ToBytes(const uint32_t data) {
    if constexpr (Q_BYTE_ORDER == Q_LITTLE_ENDIAN) {
        return QByteArray::fromRawData(reinterpret_cast<const char *>(&data),
                                       sizeof(data));
    } else {
        QByteArray bytes(sizeof(data), 0);
        qToLittleEndian<quint32>(data, bytes.data());
        return bytes;
    }
}

uint16_t bytesToInt16(const QByteArray &bytes) {
    if constexpr (Q_BYTE_ORDER == Q_LITTLE_ENDIAN) {
        return qFromBigEndian<quint16>(bytes.data());
    } else {
        return qFromLittleEndian<quint16>(bytes.data());
    }
}

uint32_t bytesToInt32(const QByteArray &bytes) {
    if constexpr (Q_BYTE_ORDER == Q_LITTLE_ENDIAN) {
        return qFromBigEndian<quint32>(bytes.data());
    } else {
        return qFromLittleEndian<quint32>(bytes.data());
    }
}