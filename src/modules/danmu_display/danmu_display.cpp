#include "danmu_display.h"

#include <QLabel>

#include "ui_danmu_window.h"

DanmuDisplay::DanmuDisplay() : Module("danmu_display") {
    config = nullptr;

    // Module UI
    QHBoxLayout *layout = new QHBoxLayout(widget);

    btn_startDisplay = new QPushButton("弹幕显示", widget);
    connect(btn_startDisplay, SIGNAL(clicked()), this, SLOT(startDisplay()));
    layout->addWidget(btn_startDisplay);

    btn_startPanel = new QPushButton("配置", widget);
    btn_startPanel->setEnabled(false);
    connect(btn_startPanel, SIGNAL(clicked()), this, SLOT(startPanel()));
    layout->addWidget(btn_startPanel);

    btn_toggleLockPosition = new QPushButton("锁定窗口", widget);
    btn_toggleLockPosition->setEnabled(false);
    connect(btn_toggleLockPosition, SIGNAL(clicked()), this,
            SLOT(toggleLockPosition()));
    layout->addWidget(btn_toggleLockPosition);
}

DanmuDisplay::~DanmuDisplay() {
    qDebug("Enter ~DanmuDisplay");
    loader->deleteLater();
    // object with attribute Qt::WA_DeleteOnClose does not need to be deleted
    // manually
    qDebug("Exit ~DanmuDisplay");
}

// slots

void DanmuDisplay::startDisplay() {
    if (!window.isNull()) {
        loader->deleteLater();
        window->deleteLater();
        btn_startPanel->setEnabled(false);
        panel->deleteLater();
        btn_toggleLockPosition->setEnabled(false);
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

    btn_toggleLockPosition->setEnabled(true);
    setLockPosition(config->lockPosition);

    btn_startPanel->setEnabled(true);

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
    if (window.isNull()) {
        return;
    }
    if (!panel.isNull()) {
        panel->deleteLater();
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

void DanmuDisplay::toggleLockPosition() {
    config->lockPosition = !config->lockPosition;
    config->save();
    setLockPosition(config->lockPosition);
}

void DanmuDisplay::setLockPosition(const bool &on) {
    window->setTransparentForMouseEvents(on);
    btn_toggleLockPosition->setText(on ? "锁定窗口 [ON]" : "锁定窗口 [OFF]");
}