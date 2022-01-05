#include "decompress.h"

#include <QDebug>

#include "brotli/decode.h"
#include "zlib/zlib.h"

// HAVE NOT TESTED YET
bool decompressDeflate(const QByteArray &data, QByteArray &result) {
    size_t encodedSize = data.size();
    size_t decodedSize = encodedSize;

    qDebug() << "Start to decompress data:" << data;

    int status;
    do {
        decodedSize <<= 1;
        result.resize(decodedSize);
        status = uncompress((Bytef *)result.data(), (uLongf *)&decodedSize,
                            (const Bytef *)data.constData(), encodedSize);
    } while (status == Z_BUF_ERROR);

    if (status == Z_OK) {
        qDebug() << "Decompressed length:" << decodedSize;
        qDebug() << "Decompressed data:" << result;
        return true;
    }
    // Error
    return false;
}

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
    // Error
    return false;
}