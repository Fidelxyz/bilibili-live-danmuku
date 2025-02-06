#include "byte_convert.h"

#include <QByteArray>
#include <cstdint>

QByteArray int16ToBytes(const uint16_t data) {
    QByteArray bytes;
    bytes.append(static_cast<char>(data >> 8 & 0xFF));
    bytes.append(static_cast<char>(data & 0xFF));
    return bytes;
}

QByteArray int32ToBytes(const uint32_t data) {
    QByteArray bytes;
    bytes.append(static_cast<char>(data >> 24 & 0xFF));
    bytes.append(static_cast<char>(data >> 16 & 0xFF));
    bytes.append(static_cast<char>(data >> 8 & 0xFF));
    bytes.append(static_cast<char>(data & 0xFF));
    return bytes;
}

uint16_t bytesToInt16(const QByteArray &bytes) {
    uint16_t data = 0;
    data |= bytes[0] << 8;
    data |= bytes[1];
    return data;
}

uint32_t bytesToInt32(const QByteArray &bytes) {
    uint32_t data = 0;
    data |= static_cast<uint8_t>(bytes[0]) << 24;
    data |= static_cast<uint8_t>(bytes[1]) << 16;
    data |= static_cast<uint8_t>(bytes[2]) << 8;
    data |= static_cast<uint8_t>(bytes[3]);
    return data;
}