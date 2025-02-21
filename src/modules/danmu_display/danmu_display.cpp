#include "danmu_display.h"

#include <QLabel>

#include "ui/ui_danmu_window.h"

DanmuDisplay::DanmuDisplay(Danmuku* parent)
    : Module("danmu_display", {"live_room"}, parent) {
    config                    = nullptr;
    updateFollowersCountTimer = nullptr;

    // Module UI
    auto* layout = new QHBoxLayout(widget);  // deleted by QT

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

    const Module* const moduleLiveRoom = getModule("live_room");
    connect(moduleLiveRoom, SIGNAL(started()), this, SLOT(enable()));
    connect(moduleLiveRoom, SIGNAL(stopped()), this, SLOT(disable()));
}

DanmuDisplay::~DanmuDisplay() {
    qDebug("Enter ~DanmuDisplay");
    stop();
    qDebug("Exit ~DanmuDisplay");
}

// slots

void DanmuDisplay::enable() const { btn_startDisplay->setEnabled(true); }

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
    Q_ASSERT(danmuLoader == nullptr);
    danmuLoader =
        new DanmuLoader(window->ui->list_danmu, this);  // deleted in stop()
    Q_ASSERT(config == nullptr);
    config = new DanmuConfig(this);  // deleted in stop()

    connect(config, SIGNAL(changed()), this, SLOT(applyConfig()));

    btn_toggleLockPosition->setEnabled(true);
    setLockPosition(config->lockPosition);

    btn_startPanel->setEnabled(true);

    applyConfig();

    danmuLoader->start();

    // Followers count
    Module* moduleLiveRoom = getModule("live_room");
    connect(moduleLiveRoom, SIGNAL(followersCountUpdated(int)), this,
            SLOT(updateFollowersCount(int)));
    QMetaObject::invokeMethod(moduleLiveRoom, "updateFollowersCount");
    Q_ASSERT(updateFollowersCountTimer == nullptr);
    updateFollowersCountTimer = new QTimer(this);  // deleted in stop()
    connect(updateFollowersCountTimer, SIGNAL(timeout()), moduleLiveRoom,
            SLOT(updateFollowersCount()));
    updateFollowersCountTimer->start(UPDATE_FOLLOWERS_COUNT_INTERVAL_MS);

    QObject* protocol;
    QMetaObject::invokeMethod(moduleLiveRoom, "getProtocol",
                              Q_RETURN_ARG(QObject*, protocol));
    connect(protocol, SIGNAL(viewersCountUpdated(int)), this,
            SLOT(updateViewersCount(int)));
    connect(protocol, SIGNAL(recvDanmu(int, QString, QString, bool, bool, int)),
            this, SLOT(recvDanmu(int, QString, QString, bool, bool, int)));
    connect(protocol, SIGNAL(recvGift(int, QString, QString, int)), this,
            SLOT(recvGift(int, QString, QString, int)));
}

void DanmuDisplay::startPanel() {
    if (!panel.isNull()) {
        panel->deleteLater();
        return;
    }
    Q_ASSERT(panel == nullptr);
    panel = new DanmuPanel(config);  // deleted in stop()

    connect(panel, &DanmuPanel::testDanmu, this, &DanmuDisplay::recvDanmu);
    connect(panel, &DanmuPanel::testGift, this, &DanmuDisplay::recvGift);
}

void DanmuDisplay::stop() {
    qDebug("Enter stop");

    if (const Module* moduleLiveRoom = getModule("live_room");
        moduleLiveRoom != nullptr) {
        disconnect(moduleLiveRoom, SIGNAL(followersCountUpdated(const int&)),
                   this, SLOT(updateFollowersCount(const int&)));
    }

    if (updateFollowersCountTimer != nullptr) {
        updateFollowersCountTimer->stop();
        delete updateFollowersCountTimer;
        updateFollowersCountTimer = nullptr;
    }

    if (!danmuLoader.isNull()) {
        delete danmuLoader;
        danmuLoader = nullptr;
    }
    if (!giftLoader.isNull()) {
        delete giftLoader;
        giftLoader = nullptr;
    }
    if (!window.isNull()) {
        delete window;
        window = nullptr;
    }
    if (!panel.isNull()) {
        delete panel;
        panel = nullptr;
    }
    if (config != nullptr) {
        delete config;
        config = nullptr;
    }

    btn_startPanel->setEnabled(false);
    btn_toggleLockPosition->setEnabled(false);
    qDebug("Exit stop");
}

void DanmuDisplay::recvDanmu(const int uid, const QString& username,
                             const QString& text, const bool isAdmin,
                             const bool isVIP, const int userGuardLevel) const {
    qDebug() << "Display danmu: " << uid << username << text << isAdmin << isVIP
             << userGuardLevel;

    QString prefix = "";
    QString suffix = "";

    if (isVIP) {
        suffix.append("<span style=\"color:yellow\">[VIP]</span>");
    }

    if (isAdmin) {
        suffix.append("<span style=\"color:yellow\">[房]</span>");
    }

    auto* label =
        new QLabel(danmuContentFormat.arg(prefix, username, suffix, text),
                   window->ui->list_danmu);  // deleted by QT

    label->setFont(config->font);
    label->setFixedWidth(window->ui->list_danmu->width());
    label->setWordWrap(true);

    auto* item = new QListWidgetItem();     // not specify parent
    window->ui->list_danmu->addItem(item);  // set parent here
    window->ui->list_danmu->setItemWidget(item, label);
    item->setSizeHint(label->sizeHint());

    danmuLoader->loadItem(item);
}

void DanmuDisplay::recvGift(const int uid, const QString& username,
                            const QString& giftName,
                            const int      giftCount) const {
    if (giftLoader.isNull()) {
        return;
    }

    qDebug() << "Display gift:" << uid << username << giftName << giftCount;

    auto* label = new QLabel(
        giftContentFormat.arg(username, giftName, QString::number(giftCount)),
        window->ui->list_gift);

    label->setFont(config->font);
    label->setFixedWidth(window->ui->list_gift->width());
    label->setWordWrap(true);

    auto* item = new QListWidgetItem();    // not specify parent
    window->ui->list_gift->addItem(item);  // set parent here
    window->ui->list_gift->setItemWidget(item, label);
    item->setSizeHint(label->sizeHint());

    giftLoader->loadItem(item);
}

void DanmuDisplay::updateViewersCount(const int& viewersCount) const {
    window->ui->label_viewersCount->setText(QString::number(viewersCount));
    qDebug() << "Update viewers count (display):" << viewersCount;
}

void DanmuDisplay::updateFollowersCount(const int& followersCount) const {
    window->ui->label_followersCount->setText(QString::number(followersCount));
    qDebug() << "Update followers count (display):" << followersCount;
}

void DanmuDisplay::applyConfig() {
    danmuContentFormat = QString(
                             "%1<span style=\"color:%2\">%3</span>%4: <span "
                             "style=\"color:%5\">%6</span>")
                             .arg("%1", config->usernameColor.name(), "%2",
                                  "%3", config->contentColor.name(), "%4");
    giftContentFormat = QString(
                            "<span style=\"color:%1\">%2</span> 送出了礼物 "
                            "<span style=\"color:%3\">%4</span> x%5")
                            .arg(config->usernameColor.name(), "%1",
                                 config->contentColor.name(), "%2", "%3");
    window->setStyleSheet(
        QString("QLabel{color:%1}").arg(config->mainColor.name()));
    window->ui->frame_window->setStyleSheet(
        QString("background-color:%1;border-radius:%2px")
            .arg(config->backgroundColor.name(),
                 QString::number(config->borderRadius)));

    window->resize(config->windowWidth, config->windowHeight);
    window->ui->frame_window->resize(config->windowWidth, config->windowHeight);
    window->setWindowOpacity(static_cast<qreal>(config->opacity) / 100);

    window->ui->prompt_followersCount->setFont(config->font);
    window->ui->prompt_viewersCount->setFont(config->font);
    window->ui->label_followersCount->setFont(config->font);
    window->ui->label_viewersCount->setFont(config->font);

    danmuLoader->setScrollingSpeed(config->scrollingSpeed, config->fps);

    QMetaObject::invokeMethod(danmuLoader, "reload");

    // giftLoader
    if (config->showGift) {
        // showGift == true
        if (giftLoader.isNull()) {
            giftLoader = new DanmuLoader(window->ui->list_gift,
                                         this);  // deleted in stop()
            giftLoader->start();
            window->ui->list_gift->show();
        }

        // apply config on giftLoader
        giftLoader->setScrollingSpeed(config->scrollingSpeed, config->fps);
        window->ui->layout_content->setStretchFactor(window->ui->list_gift,
                                                     config->giftHeightRatio);
        window->ui->layout_content->setStretchFactor(
            window->ui->list_danmu, 100 - config->giftHeightRatio);

        QMetaObject::invokeMethod(giftLoader, "reload");
    } else {
        // showGift == false
        if (!giftLoader.isNull()) {
            giftLoader->deleteLater();
            window->ui->list_gift->hide();
        }
    }
}

void DanmuDisplay::toggleLockPosition() const {
    config->lockPosition = !config->lockPosition;
    config->save();
    setLockPosition(config->lockPosition);
}

void DanmuDisplay::setLockPosition(const bool& on) const {
    window->setTransparentForMouseEvents(on);
    btn_toggleLockPosition->setText(on ? tr("锁定窗口 [ON]")
                                       : tr("锁定窗口 [OFF]"));
}