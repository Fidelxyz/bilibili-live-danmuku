#include "danmu_display.h"

#include <QLabel>
#include <QMouseEvent>

#include "ui_danmu_display.h"

DanmuDisplay::DanmuDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::DanmuDisplay) {
    ui->setupUi(this);

    setLayout(ui->layout_content);

    // Window
    // 无边框窗口
    setWindowFlags(Qt::FramelessWindowHint);
    // 忽略鼠标事件
    ui->list_danmuDisplay->setAttribute(Qt::WA_TransparentForMouseEvents);
    isMouseLeftPressed = false;

    show();

    // list
    QListWidgetItem *blankItem = new QListWidgetItem(ui->list_danmuDisplay);
    blankItem->setSizeHint(ui->list_danmuDisplay->size());
    ui->list_danmuDisplay->addItem(blankItem);

    // danmu loader
    danmuLoader = new DanmuLoader(ui->list_danmuDisplay);
    danmuLoader->slotSetScrollingSpeed(2, 30);
    danmuLoader->start();

    // default style
    fontSize = 15;
    mainColor = "white";
    usernameColor = "lightblue";
    contentColor = "white";
    backgroundColor = "black";
    setMainStyle();
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

    QString content = QString(
                          "<span style=\"color:%1\">%2</span>: <span "
                          "style=\"color:%3\">%4</span>")
                          .arg(usernameColor, username, contentColor, text);
    QLabel *label = new QLabel(content);

    label->setFixedWidth(ui->list_danmuDisplay->width());
    label->setWordWrap(true);
    QListWidgetItem *item = new QListWidgetItem(ui->list_danmuDisplay);
    ui->list_danmuDisplay->addItem(item);
    ui->list_danmuDisplay->setItemWidget(item, label);
    item->setSizeHint(label->sizeHint());

    danmuLoader->loadItem(item);
}

void DanmuDisplay::slotUpdateViewersCount(const int &viewersCount) {
    ui->label_viewersCount->setText(QString::number(viewersCount));
    qDebug() << "Update viewers count (display):" << viewersCount;
}

void DanmuDisplay::slotUpdateFollowersCount(const int &followersCount) {
    ui->label_followersCount->setText(QString::number(followersCount));
    qDebug() << "Update followers count (display):" << followersCount;
}

void DanmuDisplay::reload() {
    ui->list_danmuDisplay->clear();
    QListWidgetItem *blankItem = new QListWidgetItem(ui->list_danmuDisplay);
    blankItem->setSizeHint(ui->list_danmuDisplay->size());
    ui->list_danmuDisplay->addItem(blankItem);
    QMetaObject::invokeMethod(danmuLoader, "slotReload");
}

void DanmuDisplay::setMainStyle() {
    setStyleSheet(
        QString(
            "#DanmuDisplay{background-color:%1}QLabel{font-size:%2px;color:%3}")
            .arg(backgroundColor, QString::number(fontSize), mainColor));
}

void DanmuDisplay::slotGetWindowConfig(int *width, int *height, int *opacity) {
    *width = this->width();
    *height = this->height();
    *opacity = qRound(windowOpacity() * 100);
}

void DanmuDisplay::slotGetFontConfig(int *fontSize) {
    *fontSize = this->fontSize;
}

void DanmuDisplay::slotGetColorConfig(QColor *mainColor, QColor *usernameColor,
                                      QColor *contentColor,
                                      QColor *backgroundColor) {
    *mainColor = QColor(this->mainColor);
    *usernameColor = QColor(this->usernameColor);
    *contentColor = QColor(this->contentColor);
    *backgroundColor = QColor(this->backgroundColor);
}

void DanmuDisplay::slotSetWindowConfig(const int &width, const int &height,
                                       const int &opacity) {
    resize(width, height);
    setWindowOpacity((qreal)opacity / 100);
    reload();
}

void DanmuDisplay::slotSetFontConfig(const int &fontSize) {
    this->fontSize = fontSize;
    setMainStyle();
}

void DanmuDisplay::slotSetColorConfig(const QColor &mainColor,
                                      const QColor &usernameColor,
                                      const QColor &contentColor,
                                      const QColor &backgroundColor) {
    this->mainColor = mainColor.name();
    this->usernameColor = usernameColor.name();
    this->contentColor = contentColor.name();
    this->backgroundColor = backgroundColor.name();
    setMainStyle();
    reload();
}