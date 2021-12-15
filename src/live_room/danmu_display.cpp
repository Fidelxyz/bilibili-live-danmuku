#include "danmu_display.h"

#include <QLabel>
#include <QMouseEvent>

#include "ui_danmu_display.h"

DanmuDisplay::DanmuDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::DanmuDisplay) {
    ui->setupUi(this);

    // Window
    // 无边框窗口
    setWindowFlags(Qt::FramelessWindowHint);
    // 忽略鼠标事件
    ui->list_danmuDisplay->setAttribute(Qt::WA_TransparentForMouseEvents);
    isMouseLeftPressed = false;

    // button
    connect(ui->btn_debug, SIGNAL(clicked()), this, SLOT(slotRecvDanmuDebug()));

    // list
    QListWidgetItem *blankItem = new QListWidgetItem(ui->list_danmuDisplay);
    blankItem->setSizeHint(ui->list_danmuDisplay->size());
    ui->list_danmuDisplay->addItem(blankItem);

    // danmu loader
    danmuLoader = new DanmuLoader(ui->list_danmuDisplay);
    danmuLoader->setScrollingSpeed(2, 30);
    danmuLoader->start();
}

DanmuDisplay::~DanmuDisplay() {
    delete danmuLoader;
    delete ui;
}

// Mouse event

void DanmuDisplay::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isMouseLeftPressed = true;
        mouseDragPosition =
            event->globalPosition().toPoint() - this->frameGeometry().topLeft();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void DanmuDisplay::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isMouseLeftPressed = false;
    } else {
        QWidget::mousePressEvent(event);
    }
}

void DanmuDisplay::mouseMoveEvent(QMouseEvent *event) {
    if (isMouseLeftPressed) {
        move(event->globalPosition().toPoint() - mouseDragPosition);
    }
    QWidget::mouseMoveEvent(event);
}

void DanmuDisplay::slotRecvDanmu(const int &uid, const QString &username,
                                 const QString &text, const bool &isAdmin,
                                 const bool &isVIP, const int &userGuardLevel) {
    qDebug() << "Display: " << uid << username << text << isAdmin << isVIP
             << userGuardLevel;

    QString content = QString("<span style=\"color:lightblue;\">%1</span>: %2")
                          .arg(username, text);
    QLabel *label = new QLabel(content);

    label->setFixedWidth(ui->list_danmuDisplay->width());
    label->setWordWrap(true);
    QListWidgetItem *item = new QListWidgetItem(ui->list_danmuDisplay);
    ui->list_danmuDisplay->addItem(item);
    ui->list_danmuDisplay->setItemWidget(item, label);
    item->setSizeHint(label->sizeHint());

    danmuLoader->loadItem(item);
}

void DanmuDisplay::slotRecvDanmuDebug() {
    slotRecvDanmu(14003442, QString("XuanYun_Fidel"),
                  QString("Danmu test Danmu test Danmu test Danmu test."),
                  false, false, 233);
}

void DanmuDisplay::slotUpdateViewersCount(const int &viewersCount) {
    ui->label_viewersCount->setText(QString::number(viewersCount));
}