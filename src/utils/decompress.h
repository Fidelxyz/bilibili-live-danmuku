#ifndef UNCOMPRESS_H
#define UNCOMPRESS_H

#include <QByteArray>

bool decompressDeflate(const QByteArray &data, QByteArray &result);
bool decompressBrotli(const QByteArray &data, QByteArray &result);

#endif