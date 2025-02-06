#ifndef DANMUKU_UTILS_DECOMPRESS_H_
#define DANMUKU_UTILS_DECOMPRESS_H_

#include <QByteArray>

// bool decompressDeflate(const QByteArray &data, QByteArray &result);
bool decompressBrotli(const QByteArray &data, QByteArray *result);

#endif  // DANMUKU_UTILS_DECOMPRESS_H_
