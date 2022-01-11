#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include <QByteArray>

// bool decompressDeflate(const QByteArray &data, QByteArray &result);
bool decompressBrotli(const QByteArray &data, QByteArray &result);

#endif
