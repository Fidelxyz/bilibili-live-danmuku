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
    src/danmu_display/danmu_display.cpp \
    src/danmu_display/danmu_loader.cpp \
    src/danmu_display/danmu_panel.cpp \
    src/live_room/live_room.cpp \
    src/live_room/protocal.cpp \
    src/utils/byte_convert.cpp \
    src/utils/decompress.cpp \
    src/utils/network.cpp \
    src/main.cpp \
    src/mainwindow.cpp
    
HEADERS += \
    3rdparty/include/brotli/decode.h \
    3rdparty/include/zlib/zlib.h \
    src/danmu_display/danmu_display.h \
    src/danmu_display/danmu_loader.h \
    src/danmu_display/danmu_panel.h \
    src/live_room/live_room.h \
    src/live_room/protocal.h \
    src/utils/byte_convert.h \
    src/utils/decompress.h \
    src/utils/network.h \
    src/mainwindow.h

FORMS += \
    src/ui/danmu_display.ui \
    src/ui/danmu_panel.ui \
    src/ui/mainwindow.ui

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