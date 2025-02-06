#include "decompress.h"

#include "brotli/decode.h"
// #include "zlib/zlib.h"

// HAVE NOT BEEN TESTED YET
// bool decompressDeflate(const QByteArray& data, QByteArray& result) {
//     size_t encodedSize = data.size();
//     size_t decodedSize = encodedSize;

//     qDebug() << "Start to decompress data:" << data;

//     int status;
//     do {
//         decodedSize <<= 1;
//         result.resize(decodedSize);
//         status = uncompress((Bytef*)result.data(), (uLongf*)&decodedSize,
//                             (const Bytef*)data.constData(), encodedSize);
//     } while (status == Z_BUF_ERROR);

//     if (status == Z_OK) {
//         qDebug() << "Decompressed length:" << decodedSize;
//         qDebug() << "Decompressed data:" << result;
//         return true;
//     }
//     // Error
//     return false;
// }

bool decompressBrotli(const QByteArray& data, QByteArray* const result) {
    BrotliDecoderState* s =
        BrotliDecoderCreateInstance(nullptr, nullptr, nullptr);

    qsizetype bufSize      = data.size();
    size_t    availableIn  = bufSize;
    size_t    availableOut = bufSize;
    auto      nextIn       = reinterpret_cast<const uint8_t*>(data.constData());
    auto      nextOut      = reinterpret_cast<uint8_t*>(result->data());
    size_t    totalOut     = 0;

    // qDebug() << "Start to decompress data: " << data;

    BrotliDecoderResult status;
    do {
        availableOut += bufSize;
        bufSize <<= 1;
        result->resize(bufSize);
        nextOut = reinterpret_cast<uint8_t*>(result->data()) +
                  totalOut;  // after resize, the data address is changed
        // qDebug() << "Buffer size:" << availableOut;
        status = BrotliDecoderDecompressStream(
            s, &availableIn, &nextIn, &availableOut, &nextOut, &totalOut);
    } while (status == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT);

    BrotliDecoderDestroyInstance(s);
    result->truncate(static_cast<qsizetype>(totalOut));

    if (status == BROTLI_DECODER_RESULT_SUCCESS) {
        // qDebug() << "Decompressed length:" << totalOut;
        // qDebug() << "Decompressed data:" << result;
        return true;
    }
    // Error
    return false;
}