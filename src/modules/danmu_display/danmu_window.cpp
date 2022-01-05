#include "danmu_window.h"

#include <QMouseEvent>

#include "ui_danmu_window.h"

DanmuWindow::DanmuWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::DanmuWindow) {
    ui->setupUi(this);

    setLayout(ui->layout_content);

    // 无边框窗口
    setWindowFlags(Qt::FramelessWindowHint);
    // 忽略鼠标事件
    ui->list_danmu->setAttribute(Qt::WA_TransparentForMouseEvents);
    isMouseLeftPressed = false;

    show();
}

DanmuWindow::~DanmuWindow() { delete ui; }

// Mouse event

void DanmuWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isMouseLeftPressed = true;
        mouseDragPosition =
            event->globalPosition().toPoint() - this->frameGeometry().topLeft();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void DanmuWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isMouseLeftPressed = false;
    } else {
        QWidget::mousePressEvent(event);
    }
}

void DanmuWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isMouseLeftPressed) {
        move(event->globalPosition().toPoint() - mouseDragPosition);
    }
    QWidget::mouseMoveEvent(event);
}