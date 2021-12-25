#include "mainwindow.h"

#include <vector>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    liveRoom = nullptr;
    danmuDisplay = nullptr;
    danmuPanel = nullptr;

    connect(ui->btn_startLiveRoom, SIGNAL(clicked()), this,
            SLOT(slotStartLiveRoom()));
    connect(ui->btn_startDanmuDisplay, SIGNAL(clicked()), this,
            SLOT(slotStartDanmuDisplay()));
    connect(ui->btn_startDanmuPanel, SIGNAL(clicked()), this,
            SLOT(slotStartDanmuPanel()));
}

MainWindow::~MainWindow() {
    delete ui;
    delete liveRoom;
    delete danmuDisplay;
}

void MainWindow::slotStartLiveRoom() {
    liveRoom = new LiveRoom(ui->input_liveRoomID->text().toInt());
}

void MainWindow::slotStartDanmuDisplay() {
    if (liveRoom == nullptr || danmuDisplay != nullptr) {
        return;
    }
    danmuDisplay = new DanmuDisplay();
    connect(liveRoom, SIGNAL(updateFollowersCount(const int &)), danmuDisplay,
            SLOT(slotUpdateFollowersCount(const int &)));
    connect(liveRoom->protocal,
            SIGNAL(recvDanmu(const int &, const QString &, const QString &,
                             const bool &, const bool &, const int &)),
            danmuDisplay,
            SLOT(slotRecvDanmu(const int &, const QString &, const QString &,
                               const bool &, const bool &, const int &)));
    connect(liveRoom->protocal, SIGNAL(updateViewersCount(const int &)),
            danmuDisplay, SLOT(slotUpdateViewersCount(const int &)));

    QMetaObject::invokeMethod(liveRoom, "slotUpdateFollowersCount");
}

void MainWindow::slotStartDanmuPanel() {
    if (liveRoom == nullptr || danmuDisplay == nullptr ||
        danmuPanel != nullptr) {
        return;
    }
    danmuPanel = new DanmuPanel();
    connect(danmuPanel, SIGNAL(getWindowConfig(int *, int *, int *)),
            danmuDisplay, SLOT(slotGetWindowConfig(int *, int *, int *)));
    connect(danmuPanel, SIGNAL(getFontConfig(int *)), danmuDisplay,
            SLOT(slotGetFontConfig(int *)));
    connect(danmuPanel,
            SIGNAL(getColorConfig(QColor *, QColor *, QColor *, QColor *)),
            danmuDisplay,
            SLOT(slotGetColorConfig(QColor *, QColor *, QColor *, QColor *)));

    connect(danmuPanel,
            SIGNAL(setWindowConfig(const int &, const int &, const int &)),
            danmuDisplay,
            SLOT(slotSetWindowConfig(const int &, const int &, const int &)));
    connect(danmuPanel, SIGNAL(setFontConfig(const int &)), danmuDisplay,
            SLOT(slotSetFontConfig(const int &)));
    connect(danmuPanel,
            SIGNAL(setColorConfig(const QColor &, const QColor &,
                                  const QColor &, const QColor &)),
            danmuDisplay,
            SLOT(slotSetColorConfig(const QColor &, const QColor &,
                                    const QColor &, const QColor &)));

    connect(danmuPanel,
            SIGNAL(testDanmu(const int &, const QString &, const QString &,
                             const bool &, const bool &, const int &)),
            danmuDisplay,
            SLOT(slotRecvDanmu(const int &, const QString &, const QString &,
                               const bool &, const bool &, const int &)));
    QMetaObject::invokeMethod(danmuPanel, "slotGetConfig");
}

/*
Dependency:
liveRoom <- danmuDisplay <- danmuPanel

*/