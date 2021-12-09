#include "decompress.h"

#include <brotli/decode.h>
#include <zlib/zlib.h>

#include <QByteArray>
#include <QDebug>
#include <exception>
#include <vector>

// HAVE NOT TEST YET
bool decompressDeflate(const QByteArray &data, QByteArray &result) {
    size_t encodedSize = data.size();
    size_t decodedSize = encodedSize;
    int status;
    do {
        decodedSize <<= 1;
        result.resize(decodedSize);
        int status = uncompress((Bytef *)result.data(), (uLongf *)&decodedSize,
                                (const Bytef *)data.constData(), encodedSize);
    } while (status == Z_BUF_ERROR);

    if (status == Z_OK) {
        qDebug() << "Uncompressed length: " << decodedSize;
        return true;
    }
    return false;
}

// bool decompressDeflate(const QByteArray &data, QByteArray &result) {
//     size_t bufSize = data.size() << 1;
//     while (true) {
//         std::vector<uint8_t> buf(bufSize);
//         // uLongf uncompressLen = buffer.size();
//         uLongf uncompressLen = 0;
//         int status = uncompress(
//             buf.data(), &uncompressLen,
//             reinterpret_cast<const Bytef *>(data.constData()), data.size());
//         if (status == Z_OK) {
//             result = QByteArray(reinterpret_cast<const char *>(buf.data()),
//                                 buf.size());
//             qDebug() << "Uncompressed length: " << uncompressLen;
//             return true;
//         }
//         if (status == Z_BUF_ERROR) {
//             bufSize <<= 1;
//             continue;
//         }
//         return false;  // Other error
//     }
// }

bool decompressBrotli(const QByteArray &data, QByteArray &result) {
    BrotliDecoderState *s = BrotliDecoderCreateInstance(NULL, NULL, NULL);

    size_t bufSize = data.size();
    size_t availableIn = bufSize;
    size_t availableOut = bufSize;
    const uint8_t *nextIn = (const uint8_t *)data.constData();
    uint8_t *nextOut = (uint8_t *)result.data();
    size_t totalOut = 0;

    qDebug() << "Start to decompress data: " << data;

    BrotliDecoderResult status;
    do {
        availableOut += bufSize;
        bufSize <<= 1;
        result.resize(bufSize);
        nextOut = ((uint8_t *)result.data()) +
                  totalOut;  // after resize, the data address is changed
        qDebug() << "Buffer size:" << availableOut;
        status = BrotliDecoderDecompressStream(
            s, &availableIn, &nextIn, &availableOut, &nextOut, &totalOut);
    } while (status == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT);

    BrotliDecoderDestroyInstance(s);
    result.truncate(totalOut);

    if (status == BROTLI_DECODER_RESULT_SUCCESS) {
        qDebug() << "Decompressed length:" << totalOut;
        qDebug() << "Decompressed data:" << result;
        return true;
    }
    return false;
}