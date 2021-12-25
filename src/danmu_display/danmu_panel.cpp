#include "danmu_panel.h"

#include <QColorDialog>

#include "ui_danmu_panel.h"

DanmuPanel::DanmuPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::DanmuPanel) {
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

    show();
}

DanmuPanel::~DanmuPanel() {}

void DanmuPanel::slotGetConfig() {
    int width = 0, height = 0, opacity = 0;
    emit getWindowConfig(&width, &height, &opacity);
    ui->spin_windowWidth->setValue(width);
    ui->spin_windowHeight->setValue(height);
    ui->spin_opacity->setValue(opacity);

    int fontSize = 0;
    emit getFontConfig(&fontSize);
    ui->spin_fontSize->setValue(fontSize);

    emit getColorConfig(&mainColor, &usernameColor, &contentColor,
                        &backgroundColor);

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
    emit setWindowConfig(ui->spin_windowWidth->value(),
                         ui->spin_windowHeight->value(),
                         ui->spin_opacity->value());
    emit setFontConfig(ui->spin_fontSize->value());
    emit setColorConfig(mainColor, usernameColor, contentColor,
                        backgroundColor);
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