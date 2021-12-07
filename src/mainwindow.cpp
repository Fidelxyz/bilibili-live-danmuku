#include "mainwindow.h"

#include <vector>

#include "live_room/danmu.h"
#include "live_room/live_room.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->btn_ConnectLiveRoom, SIGNAL(clicked()), this,
            SLOT(click_btn_ConnectLiveRoom()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::click_btn_ConnectLiveRoom() {
    ui->textBrowser->setText("Clicked!");
    LiveRoom *liveRoom = new LiveRoom(ui->input_LiveRoomID->text().toInt());
}