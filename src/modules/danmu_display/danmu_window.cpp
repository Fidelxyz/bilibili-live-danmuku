#include "danmu_window.h"

#include <QMouseEvent>

#include "ui_danmu_window.h"

DanmuWindow::DanmuWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::DanmuWindow) {
    isMouseLeftPressed = false;

    ui->setupUi(this);

    setLayout(ui->layout_content);
    setWindowFlag(Qt::FramelessWindowHint);   // frameless window
    setWindowFlag(Qt::WindowStaysOnTopHint);  // Always on top
    setWindowFlag(Qt::Tool);                  // hide docker icon
    setAttribute(Qt::WA_DeleteOnClose);       // delete when closing window

    ui->list_danmu->setAttribute(
        Qt::WA_TransparentForMouseEvents);  // ignore mouse event

    show();
}

DanmuWindow::~DanmuWindow() {
    qDebug("Enter ~DanmuWindow");
    delete ui;
    qDebug("Exit ~DanmuWindow");
}

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

#ifdef Q_OS_WIN
#include <windows.h>
#endif

void DanmuWindow::setTransparentForMouseEvents(const bool &on) {
#ifdef Q_OS_WIN
    if (on) {
        SetWindowLong(
            (HWND)winId(), GWL_EXSTYLE,
            GetWindowLong((HWND)winId(), GWL_EXSTYLE) | WS_EX_TRANSPARENT);
    } else {
        SetWindowLong(
            (HWND)winId(), GWL_EXSTYLE,
            GetWindowLong((HWND)winId(), GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
    }
#endif
}