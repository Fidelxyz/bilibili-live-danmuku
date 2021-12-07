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