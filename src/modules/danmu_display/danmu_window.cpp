#include "danmu_window.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QFile>
#include <QMouseEvent>

#include "ui/ui_danmu_window.h"

#ifdef Q_CC_MSVC
#pragma comment(lib, "user32.lib")
#endif

DanmuWindow::DanmuWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::DanmuWindow) {  // deleted in ~DanmuWindow
    isMouseLeftPressed = false;

    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);   // frameless window
    setWindowFlag(Qt::WindowStaysOnTopHint);  // Always on top
    // setWindowFlag(Qt::Tool);                     // hide docker icon
    setAttribute(Qt::WA_DeleteOnClose);          // delete when closing window
    setAttribute(Qt::WA_TranslucentBackground);  // transparent background

    ui->frame_window->setLayout(ui->layout_content);
    ui->frame_window->move(0, 0);

    // ignore mouse event
    ui->list_danmu->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->list_gift->setAttribute(Qt::WA_TransparentForMouseEvents);

    show();
}

DanmuWindow::~DanmuWindow() {
    qDebug("Enter ~DanmuWindow");
    ui->list_danmu->clear();
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

void DanmuWindow::setTransparentForMouseEvents(const bool &on) {
    // TODO: Not working on macOS
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
