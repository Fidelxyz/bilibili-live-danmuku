#include "byte_convert.h"

#include <QByteArray>
#include <cstdint>
#include <cstring>

QByteArray int16ToBytes(uint16_t data) {
    QByteArray bytes = QByteArray();
    bytes.append((data >> 8) & 0xFF);
    bytes.append(data & 0xFF);
    return bytes;
}

QByteArray int32ToBytes(uint32_t data) {
    QByteArray bytes = QByteArray();
    bytes.append((data >> 24) & 0xFF);
    bytes.append((data >> 16) & 0xFF);
    bytes.append((data >> 8) & 0xFF);
    bytes.append(data & 0xFF);
    return bytes;
}

uint16_t bytesToInt16(QByteArray bytes) {
    uint16_t data = 0;
    data |= bytes[0] << 8;
    data |= bytes[1];
    return data;
}

uint32_t bytesToInt32(QByteArray bytes) {
    uint32_t data = 0;
    data |= ((uint8_t)bytes[0]) << 24;
    data |= ((uint8_t)bytes[1]) << 16;
    data |= ((uint8_t)bytes[2]) << 8;
    data |= ((uint8_t)bytes[3]);
    return data;
}