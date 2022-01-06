#include "danmu_display.h"

#include <QLabel>

#include "ui_danmu_window.h"

DanmuDisplay::DanmuDisplay() : Module("danmu_display") {
    window = nullptr;
    loader = nullptr;
    panel = nullptr;
    config = nullptr;

    // Module UI
    QHBoxLayout *layout = new QHBoxLayout(widget);

    btn_startDisplay = new QPushButton("弹幕显示", widget);
    connect(btn_startDisplay, SIGNAL(clicked()), this, SLOT(startDisplay()));
    layout->addWidget(btn_startDisplay);

    btn_startPanel = new QPushButton("配置", widget);
    connect(btn_startPanel, SIGNAL(clicked()), this, SLOT(startPanel()));
    layout->addWidget(btn_startPanel);

    btn_toggleOnTop = new QPushButton("窗口置顶", widget);
    btn_toggleOnTop->setEnabled(false);
    connect(btn_toggleOnTop, SIGNAL(clicked()), this, SLOT(toggleOnTop()));
    layout->addWidget(btn_toggleOnTop);
}

DanmuDisplay::~DanmuDisplay() {
    qDebug("Enter ~DanmuDisplay");
    delete loader;
    delete window;
    delete panel;
    qDebug("Exit ~DanmuDisplay");
}

// slots

void DanmuDisplay::startDisplay() {
    if (window != nullptr) {
        window->show();
        return;
    }

    bool isLiveRoomRunning;
    QMetaObject::invokeMethod(getModule("live_room"), "isRunning",
                              Q_RETURN_ARG(bool, isLiveRoomRunning));
    if (!isLiveRoomRunning) {
        return;
    }

    window = new DanmuWindow();

    danmuList = window->ui->list_danmu;

    // config
    config = new DanmuConfig(this);
    connect(config, SIGNAL(changed()), this, SLOT(applyConfig()));

    btn_toggleOnTop->setEnabled(true);
    setOnTop(config->onTop);

    // list
    // QListWidgetItem *blankItem = new QListWidgetItem(danmuList);
    // blankItem->setSizeHint(danmuList->size());
    // danmuList->addItem(blankItem);

    // danmu loader
    loader = new DanmuLoader(danmuList);
    loader->setScrollingSpeed(2, 30);
    loader->start();

    applyConfig();

    // Followers count
    Module *moduleLiveRoom = getModule("live_room");
    connect(moduleLiveRoom, SIGNAL(followersCountUpdated(const int &)), this,
            SLOT(updateFollowersCount(const int &)));
    QMetaObject::invokeMethod(moduleLiveRoom, "updateFollowersCount");
    updateFollowersCountTimer = new QTimer(this);
    connect(updateFollowersCountTimer, SIGNAL(timeout()), moduleLiveRoom,
            SLOT(updateFollowersCount()));
    updateFollowersCountTimer->start(UPDATE_FOLLOWERS_COUNT_INTERVAL_MS);
}

void DanmuDisplay::startPanel() {
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
            SLOT(recvDanmu(const int &, const QString &, const QString &,
                           const bool &, const bool &, const int &)));
}

void DanmuDisplay::recvDanmu(const int &uid, const QString &username,
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

void DanmuDisplay::updateViewersCount(const int &viewersCount) {
    window->ui->label_viewersCount->setText(QString::number(viewersCount));
    qDebug() << "Update viewers count (display):" << viewersCount;
}

void DanmuDisplay::updateFollowersCount(const int &followersCount) {
    window->ui->label_followersCount->setText(QString::number(followersCount));
    qDebug() << "Update followers count (display):" << followersCount;
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

    // reload
    danmuList->clear();
    QListWidgetItem *blankItem = new QListWidgetItem(danmuList);
    blankItem->setSizeHint(danmuList->size());
    danmuList->addItem(blankItem);
    QMetaObject::invokeMethod(loader, "reload");
}

void DanmuDisplay::toggleOnTop() {
    config->onTop = !config->onTop;
    config->save();
    setOnTop(config->onTop);
}

void DanmuDisplay::setOnTop(const bool &on) {
    window->setWindowFlag(Qt::WindowStaysOnTopHint, on);
    window->show();
    btn_toggleOnTop->setText(on ? "窗口置顶 [ON]" : "窗口置顶 [OFF]");
}