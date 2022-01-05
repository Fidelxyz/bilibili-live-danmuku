#include "danmu_display.h"

#include <QLabel>

#include "ui_danmu_window.h"

DanmuDisplay::DanmuDisplay(QWidget *parent) : Module(parent) {
    window = nullptr;
    loader = nullptr;
    panel = nullptr;
    config = nullptr;

    // Control UI
    QHBoxLayout *layout = new QHBoxLayout(widget);

    btn_startDisplay = new QPushButton("弹幕显示", widget);
    connect(btn_startDisplay, SIGNAL(clicked()), this,
            SLOT(slotStartDisplay()));
    layout->addWidget(btn_startDisplay);

    btn_startPanel = new QPushButton("配置", widget);
    connect(btn_startPanel, SIGNAL(clicked()), this, SLOT(slotStartPanel()));
    layout->addWidget(btn_startPanel);
}

DanmuDisplay::~DanmuDisplay() {
    delete loader;
    delete window;
    delete panel;
}

// slots

void DanmuDisplay::slotStartDisplay() {
    if (window != nullptr) {
        window->show();
        return;
    }

    window = new DanmuWindow();

    danmuList = window->ui->list_danmu;

    // config
    config = new DanmuConfig(this);
    connect(config, SIGNAL(changed()), this, SLOT(slotSetConfig()));

    // list
    QListWidgetItem *blankItem = new QListWidgetItem(danmuList);
    blankItem->setSizeHint(danmuList->size());
    danmuList->addItem(blankItem);

    // danmu loader
    loader = new DanmuLoader(danmuList);
    loader->slotSetScrollingSpeed(2, 30);
    loader->start();

    applyConfig();
}

void DanmuDisplay::slotStartPanel() {
    if (window == nullptr) {
        return;
    }
    if (panel != nullptr) {
        panel->show();
        return;
    }
    panel = new DanmuPanel(config);

    connect(panel,
            SIGNAL(testDanmu(const int &, const QString &, const QString &,
                             const bool &, const bool &, const int &)),
            this,
            SLOT(slotRecvDanmu(const int &, const QString &, const QString &,
                               const bool &, const bool &, const int &)));
}

void DanmuDisplay::slotRecvDanmu(const int &uid, const QString &username,
                                 const QString &text, const bool &isAdmin,
                                 const bool &isVIP, const int &userGuardLevel) {
    qDebug() << "Display: " << uid << username << text << isAdmin << isVIP
             << userGuardLevel;

    QLabel *label = new QLabel(contentFormat.arg(username, text));

    label->setFixedWidth(danmuList->width());
    label->setWordWrap(true);
    QListWidgetItem *item = new QListWidgetItem(danmuList);
    danmuList->addItem(item);
    danmuList->setItemWidget(item, label);
    item->setSizeHint(label->sizeHint());

    loader->loadItem(item);
}

void DanmuDisplay::slotUpdateViewersCount(const int &viewersCount) {
    window->ui->label_viewersCount->setText(QString::number(viewersCount));
    qDebug() << "Update viewers count (display):" << viewersCount;
}

void DanmuDisplay::slotUpdateFollowersCount(const int &followersCount) {
    window->ui->label_followersCount->setText(QString::number(followersCount));
    qDebug() << "Update followers count (display):" << followersCount;
}

void DanmuDisplay::reload() {
    danmuList->clear();
    QListWidgetItem *blankItem = new QListWidgetItem(danmuList);
    blankItem->setSizeHint(danmuList->size());
    danmuList->addItem(blankItem);
    QMetaObject::invokeMethod(loader, "slotReload");
}

void DanmuDisplay::applyConfig() {
    contentFormat = QString(
                        "<span style=\"color:%1\">%2</span>: <span "
                        "style=\"color:%3\">%4</span>")
                        .arg(config->usernameColor.name(), "%1",
                             config->contentColor.name(), "%2");
    window->setStyleSheet(
        QString(
            "#DanmuWindow{background-color:%1}QLabel{font-size:%2px;color:%3}")
            .arg(config->backgroundColor.name(),
                 QString::number(config->fontSize), config->mainColor.name()));
    window->resize(config->windowWidth, config->windowHeight);
    window->setWindowOpacity((qreal)config->opacity / 100);
}

void DanmuDisplay::slotSetConfig() {
    applyConfig();
    reload();
}

void DanmuDisplay::slotSetOnTop(const bool &on) {
    window->setWindowFlag(Qt::WindowStaysOnTopHint, on);
}