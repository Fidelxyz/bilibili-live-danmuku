#include "danmu_display.h"

#include <QLabel>

#include "ui_danmu_window.h"

DanmuDisplay::DanmuDisplay() : Module("danmu_display") {
    config = nullptr;
    updateFollowersCountTimer = nullptr;

    // Module UI
    QHBoxLayout *layout = new QHBoxLayout(widget);  // deleted by QT

    btn_startDisplay =
        new QPushButton(tr("弹幕显示"), widget);  // deleted by QT
    btn_startDisplay->setEnabled(false);
    connect(btn_startDisplay, SIGNAL(clicked()), this, SLOT(startDisplay()));
    layout->addWidget(btn_startDisplay);

    btn_startPanel = new QPushButton(tr("配置"), widget);  // deleted by QT
    btn_startPanel->setEnabled(false);
    connect(btn_startPanel, SIGNAL(clicked()), this, SLOT(startPanel()));
    layout->addWidget(btn_startPanel);

    btn_toggleLockPosition =
        new QPushButton(tr("锁定窗口"), widget);  // deleted by QT
    btn_toggleLockPosition->setEnabled(false);
    connect(btn_toggleLockPosition, SIGNAL(clicked()), this,
            SLOT(toggleLockPosition()));
    layout->addWidget(btn_toggleLockPosition);

    Module *moduleLiveRoom = getModule("live_room");
    connect(moduleLiveRoom, SIGNAL(started()), this, SLOT(enable()));
    connect(moduleLiveRoom, SIGNAL(stopped()), this, SLOT(disable()));
}

DanmuDisplay::~DanmuDisplay() {
    qDebug("Enter ~DanmuDisplay");
    stop();
    qDebug("Exit ~DanmuDisplay");
}

// slots

void DanmuDisplay::enable() { btn_startDisplay->setEnabled(true); }

void DanmuDisplay::disable() {
    btn_startDisplay->setEnabled(false);
    stop();
}

void DanmuDisplay::startDisplay() {
    if (!window.isNull()) {
        stop();
        return;
    }

    Q_ASSERT(window == nullptr);
    window = new DanmuWindow();  // deleted in stop()
    Q_ASSERT(loader == nullptr);
    loader = new DanmuLoader(window->ui->list_danmu);  // deleted in stop()
    Q_ASSERT(config == nullptr);
    config = new DanmuConfig();  // deleted in stop()

    connect(config, SIGNAL(changed()), this, SLOT(applyConfig()));

    btn_toggleLockPosition->setEnabled(true);
    setLockPosition(config->lockPosition);

    btn_startPanel->setEnabled(true);

    applyConfig();

    loader->start();

    // Followers count
    Module *moduleLiveRoom = getModule("live_room");
    connect(moduleLiveRoom, SIGNAL(followersCountUpdated(const int &)), this,
            SLOT(updateFollowersCount(const int &)));
    QMetaObject::invokeMethod(moduleLiveRoom, "updateFollowersCount");
    Q_ASSERT(updateFollowersCountTimer == nullptr);
    updateFollowersCountTimer = new QTimer(this);  // deleted in stop()
    connect(updateFollowersCountTimer, SIGNAL(timeout()), moduleLiveRoom,
            SLOT(updateFollowersCount()));
    updateFollowersCountTimer->start(UPDATE_FOLLOWERS_COUNT_INTERVAL_MS);
}

void DanmuDisplay::startPanel() {
    if (!panel.isNull()) {
        panel->deleteLater();
        return;
    }
    Q_ASSERT(panel == nullptr);
    panel = new DanmuPanel(config);  // deleted in stop()

    connect(panel,
            SIGNAL(testDanmu(const int &, const QString &, const QString &,
                             const bool &, const bool &, const int &)),
            this,
            SLOT(recvDanmu(const int &, const QString &, const QString &,
                           const bool &, const bool &, const int &)));
}

void DanmuDisplay::stop() {
    qDebug("Enter stop");

    Module *moduleLiveRoom = getModule("live_room");
    if (moduleLiveRoom != nullptr) {
        disconnect(moduleLiveRoom, SIGNAL(followersCountUpdated(const int &)),
                   this, SLOT(updateFollowersCount(const int &)));
    }

    if (updateFollowersCountTimer != nullptr) {
        updateFollowersCountTimer->stop();
        updateFollowersCountTimer->deleteLater();
        updateFollowersCountTimer = nullptr;
    }

    if (!loader.isNull()) {
        loader->deleteLater();
    }
    if (!window.isNull()) {
        window->deleteLater();
    }
    if (!panel.isNull()) {
        panel->deleteLater();
    }
    if (config != nullptr) {
        config->deleteLater();
        config = nullptr;
    }

    btn_startPanel->setEnabled(false);
    btn_toggleLockPosition->setEnabled(false);
    qDebug("Exit stop");
}

void DanmuDisplay::recvDanmu(const int &uid, const QString &username,
                             const QString &text, const bool &isAdmin,
                             const bool &isVIP, const int &userGuardLevel) {
    qDebug() << "Display: " << uid << username << text << isAdmin << isVIP
             << userGuardLevel;

    QString prefix = "";
    QString suffix = "";

    if (isVIP) {
        suffix.append("<span style=\"color:yellow\">[VIP]</span>");
    }

    if (isAdmin) {
        // suffix.append("<span
        // style=\"padding-left:10px;padding-right:10px;border-radius:10px;background-color:yellow;\">房</span>");
        suffix.append("<span style=\"color:yellow\">[房]</span>");
    }

    QLabel *label =
        new QLabel(contentFormat.arg(prefix, username, suffix, text),
                   window->ui->list_danmu);  // deleted by QT

    label->setFont(config->font);
    label->setFixedWidth(window->ui->list_danmu->width());
    label->setWordWrap(true);

    QListWidgetItem *item =
        new QListWidgetItem(window->ui->list_danmu);  // deleted by QT
    window->ui->list_danmu->addItem(item);
    window->ui->list_danmu->setItemWidget(item, label);
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
                        "%1<span style=\"color:%2\">%3</span>%4: <span "
                        "style=\"color:%5\">%6</span>")
                        .arg("%1", config->usernameColor.name(), "%2", "%3",
                             config->contentColor.name(), "%4");
    window->setStyleSheet(
        QString("QLabel{color:%1}").arg(config->mainColor.name()));
    window->ui->frame_window->setStyleSheet(
        QString("background-color:%1;border-radius:%2px")
            .arg(config->backgroundColor.name(),
                 QString::number(config->borderRadius)));
    window->resize(config->windowWidth, config->windowHeight);
    window->ui->frame_window->resize(config->windowWidth, config->windowHeight);
    window->setWindowOpacity((qreal)config->opacity / 100);

    window->ui->prompt_followersCount->setFont(config->font);
    window->ui->prompt_viewersCount->setFont(config->font);
    window->ui->label_followersCount->setFont(config->font);
    window->ui->label_viewersCount->setFont(config->font);

    loader->setScrollingSpeed(config->scrollingSpeed, config->fps);

    // load / reload window->ui->list_danmu
    window->ui->list_danmu->clear();
    QListWidgetItem *blankItem =
        new QListWidgetItem(window->ui->list_danmu);  // deleted by QT
    blankItem->setSizeHint(window->ui->list_danmu->size());
    window->ui->list_danmu->addItem(blankItem);
    QMetaObject::invokeMethod(loader, "reload");
}

void DanmuDisplay::toggleLockPosition() {
    config->lockPosition = !config->lockPosition;
    config->save();
    setLockPosition(config->lockPosition);
}

void DanmuDisplay::setLockPosition(const bool &on) {
    window->setTransparentForMouseEvents(on);
    btn_toggleLockPosition->setText(on ? tr("锁定窗口 [ON]")
                                       : tr("锁定窗口 [OFF]"));
}