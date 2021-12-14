QT += \
    core gui \
    network \
    websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/3rdparty/include

SOURCES += \
    src/live_room/danmu_display.cpp \
    src/live_room/danmu_loader.cpp \
    src/live_room/live_room.cpp \
    src/live_room/protocal.cpp \
    src/utils/byte_convert.cpp \
    src/utils/decompress.cpp \
    src/main.cpp \
    src/mainwindow.cpp
    
HEADERS += \
    3rdparty/include/brotli/decode.h \
    3rdparty/include/zlib/zlib.h \
    src/live_room/danmu_display.h \
    src/live_room/danmu_loader.h \
    src/live_room/live_room.h \
    src/live_room/protocal.h \
    src/utils/byte_convert.h \
    src/utils/decompress.h \
    src/mainwindow.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/danmu_display.ui

LIBS += \
    -L$$PWD/3rdparty/bin/brotli -llibbrotlidec \
    -L$$PWD/3rdparty/bin/zlib -llibzlib

RESOURCES += \
    src/stylesheet/stylesheet.qrc

TRANSLATIONS += \
    src/bilibili-live-danmuku_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# For debug
#搜寻该路径底下的文件，举例Src Src2 Src3... 后续有需要可以添加更多
#SourceDir = $$PWD/3rdparty/brotli

#递归搜寻所有.cpp .h .ui文件
#for(var, SourceDir) {
#    SOURCES += $$files($$join(var, , , /*.c), true)
#    HEADERS += $$files($$join(var, , , /*.h), true)
#}