#include "danmu.h"

#include "ui_danmu.h"

Danmu::Danmu(QWidget *parent) : QWidget(parent), ui(new Ui::Danmu) {
    ui->setupUi(this);
}

Danmu::~Danmu() { delete ui; }