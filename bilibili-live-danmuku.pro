QT += \
    core gui \
    network \
    websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

mingw:QMAKE_CXXFLAGS_RELEASE += -O3

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/3rdparty/include

SOURCES += \
    src/modules/danmu_display/danmu_config.cpp \
    src/modules/danmu_display/danmu_display.cpp \
    src/modules/danmu_display/danmu_loader.cpp \
    src/modules/danmu_display/danmu_panel.cpp \
    src/modules/danmu_display/danmu_window.cpp \
    src/modules/live_room/live_room.cpp \
    src/modules/live_room/protocal.cpp \
    src/utils/byte_convert.cpp \
    src/utils/decompress.cpp \
    src/utils/network.cpp \
    src/danmuku.cpp \
    src/main.cpp \
    src/module.cpp
    
HEADERS += \
    3rdparty/include/brotli/decode.h \
    src/modules/danmu_display/danmu_config.h \
    src/modules/danmu_display/danmu_display.h \
    src/modules/danmu_display/danmu_loader.h \
    src/modules/danmu_display/danmu_panel.h \
    src/modules/danmu_display/danmu_window.h \
    src/modules/live_room/live_room.h \
    src/modules/live_room/protocal.h \
    src/utils/byte_convert.h \
    src/utils/decompress.h \
    src/utils/network.h \
    src/danmuku.h \
    src/global.h \
    src/module.h

FORMS += \
    src/modules/danmu_display/ui/danmu_panel.ui \
    src/modules/danmu_display/ui/danmu_window.ui \
    src/ui/danmuku.ui

LIBS += \
    -L$$PWD/3rdparty/lib -lbrotlidec

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
