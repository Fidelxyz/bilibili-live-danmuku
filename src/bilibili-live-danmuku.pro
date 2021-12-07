QT += \
    core gui \
    network \
    websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    live_room/danmu.cpp \
    live_room/live_room.cpp \
    main.cpp \
    mainwindow.cpp \
    utils/byte_convert.cpp
    

HEADERS += \
    live_room/danmu.h \
    live_room/live_room.h \
    mainwindow.h \
    utils/byte_convert.h

FORMS += \
    ui/mainwindow.ui

TRANSLATIONS += \
    bilibili-live-danmuku_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
