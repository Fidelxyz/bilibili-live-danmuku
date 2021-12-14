#include "mainwindow.h"

#include <vector>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->btn_ConnectLiveRoom, SIGNAL(clicked()), this,
            SLOT(click_btn_ConnectLiveRoom()));
}

MainWindow::~MainWindow() {
    delete ui;
    delete liveRoom;
}

void MainWindow::click_btn_ConnectLiveRoom() {
    ui->textBrowser->setText("Clicked!");
    liveRoom = new LiveRoom(ui->input_LiveRoomID->text().toInt());
}
