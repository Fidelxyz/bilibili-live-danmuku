#ifndef BYTE_CONVERT_H
#define BYTE_CONVERT_H

#include <QByteArray>
#include <cstdint>

QByteArray int16ToBytes(uint16_t data);
QByteArray int32ToBytes(uint32_t data);
uint16_t bytesToInt16(QByteArray bytes);
uint32_t bytesToInt32(QByteArray bytes);

#endif