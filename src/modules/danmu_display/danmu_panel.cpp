#include "danmu_panel.h"

#include <QColorDialog>

#include "ui_danmu_panel.h"

DanmuPanel::DanmuPanel(DanmuConfig *config, QWidget *parent)
    : QWidget(parent), ui(new Ui::DanmuPanel), config(config) {
    ui->setupUi(this);

    connect(ui->btn_mainColor, SIGNAL(clicked()), this,
            SLOT(slotSetMainColor()));
    connect(ui->btn_usernameColor, SIGNAL(clicked()), this,
            SLOT(slotSetUsernameColor()));
    connect(ui->btn_contentColor, SIGNAL(clicked()), this,
            SLOT(slotSetContentColor()));
    connect(ui->btn_backgroundColor, SIGNAL(clicked()), this,
            SLOT(slotSetBackgroundColor()));

    connect(ui->btn_apply, SIGNAL(clicked()), this, SLOT(slotApply()));

    connect(ui->btn_testDanmu, SIGNAL(clicked()), this, SLOT(slotTestDanmu()));

    loadConfig();

    show();
}

DanmuPanel::~DanmuPanel() {}

void DanmuPanel::loadConfig() {
    ui->spin_windowWidth->setValue(config->windowWidth);
    ui->spin_windowHeight->setValue(config->windowHeight);
    ui->spin_opacity->setValue(config->opacity);
    ui->spin_fontSize->setValue(config->fontSize);
    mainColor = config->mainColor;
    usernameColor = config->usernameColor;
    contentColor = config->contentColor;
    backgroundColor = config->backgroundColor;

#define SET_BTN_STYLE_SHEET(NAME)  \
    ui->btn_##NAME->setStyleSheet( \
        QString("background-color:%1").arg(NAME.name()))

    SET_BTN_STYLE_SHEET(mainColor);
    SET_BTN_STYLE_SHEET(usernameColor);
    SET_BTN_STYLE_SHEET(contentColor);
    SET_BTN_STYLE_SHEET(backgroundColor);

#undef SET_BTN_STYLE_SHEET
}

void DanmuPanel::slotApply() {
    qDebug("slotApply");
    config->windowWidth = ui->spin_windowWidth->value();
    config->windowHeight = ui->spin_windowHeight->value();
    config->opacity = ui->spin_opacity->value();
    config->fontSize = ui->spin_fontSize->value();
    config->mainColor = mainColor;
    config->usernameColor = usernameColor;
    config->contentColor = contentColor;
    config->backgroundColor = backgroundColor;
    config->apply();
    config->save();
}

void DanmuPanel::slotTestDanmu() {
    emit testDanmu(14003442, QString("XuanYun_Fidel"),
                   QString("Danmu test Danmu test Danmu test Danmu test."),
                   true, false, 233);
}

#define SET_COLOR(NAME)                        \
    NAME = QColorDialog::getColor(NAME, this), \
    ui->btn_##NAME->setStyleSheet(             \
        QString("background-color:%1").arg(NAME.name()))

void DanmuPanel::slotSetMainColor() { SET_COLOR(mainColor); }

void DanmuPanel::slotSetUsernameColor() { SET_COLOR(usernameColor); }

void DanmuPanel::slotSetContentColor() { SET_COLOR(contentColor); }

void DanmuPanel::slotSetBackgroundColor() { SET_COLOR(backgroundColor); }

#undef SET_COLOR