#include "danmu_panel.h"

#include <QColorDialog>
#include <QFontDialog>

#include "ui_danmu_panel.h"

DanmuPanel::DanmuPanel(DanmuConfig *config, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::DanmuPanel),  // deleted in ~DanmuPanel
      config(config) {
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::WindowMinMaxButtonsHint,
                  false);  // disable minimize / maximize button
    setFixedSize(size());
    setLayout(ui->layout_panel);

    connect(ui->btn_font, SIGNAL(clicked()), this, SLOT(setFont()));
    connect(ui->spin_fontSize, SIGNAL(valueChanged(int)), this,
            SLOT(setFontSize(const int &)));
    connect(ui->btn_mainColor, SIGNAL(clicked()), this, SLOT(setMainColor()));
    connect(ui->btn_usernameColor, SIGNAL(clicked()), this,
            SLOT(setUsernameColor()));
    connect(ui->btn_contentColor, SIGNAL(clicked()), this,
            SLOT(setContentColor()));
    connect(ui->btn_backgroundColor, SIGNAL(clicked()), this,
            SLOT(setBackgroundColor()));

    ui->combo_fps->addItems({"15", "30", "60"});

    connect(ui->btn_apply, SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->btn_setToDefault, SIGNAL(clicked()), this,
            SLOT(setToDefault()));
    connect(ui->btn_cancelChange, SIGNAL(clicked()), this, SLOT(loadConfig()));

    connect(ui->btn_testDanmu, SIGNAL(clicked()), this, SLOT(testDanmu()));

    loadConfig();

    show();
}

DanmuPanel::~DanmuPanel() {
    qDebug() << "Enter ~DanmuPanel";
    delete ui;
    qDebug() << "Exit ~DanmuPanel";
}

void DanmuPanel::loadConfig() {
    ui->spin_windowWidth->setValue(config->windowWidth);
    ui->spin_windowHeight->setValue(config->windowHeight);
    ui->spin_opacity->setValue(config->opacity);
    ui->spin_borderRadius->setValue(config->borderRadius);
    font = config->font;
    ui->label_font->setText(
        QString("%1 %2").arg(font.family(), font.styleName()));
    ui->spin_fontSize->setValue(font.pointSize());
    mainColor = config->mainColor;
    usernameColor = config->usernameColor;
    contentColor = config->contentColor;
    backgroundColor = config->backgroundColor;
    ui->spin_scrollingSpeed->setValue(config->scrollingSpeed);
    ui->combo_fps->setCurrentText(QString::number(config->fps));
    ui->slider_fps->setValue(
        ui->combo_fps->findText(ui->combo_fps->currentText()));

#define SET_BTN_STYLE_SHEET(NAME)  \
    ui->btn_##NAME->setStyleSheet( \
        QString("background-color:%1").arg(NAME.name()))

    SET_BTN_STYLE_SHEET(mainColor);
    SET_BTN_STYLE_SHEET(usernameColor);
    SET_BTN_STYLE_SHEET(contentColor);
    SET_BTN_STYLE_SHEET(backgroundColor);

#undef SET_BTN_STYLE_SHEET
}

void DanmuPanel::apply() {
    qDebug("apply");
    config->windowWidth = ui->spin_windowWidth->value();
    config->windowHeight = ui->spin_windowHeight->value();
    config->opacity = ui->spin_opacity->value();
    config->borderRadius = ui->spin_borderRadius->value();
    config->font = font;
    config->mainColor = mainColor;
    config->usernameColor = usernameColor;
    config->contentColor = contentColor;
    config->backgroundColor = backgroundColor;
    config->scrollingSpeed = ui->spin_scrollingSpeed->value();
    config->fps = ui->combo_fps->currentText().toInt();
    config->apply();
    config->save();
}

void DanmuPanel::setToDefault() {
    ui->spin_windowWidth->setValue(DEFAULT_CONFIG::windowWidth);
    ui->spin_windowHeight->setValue(DEFAULT_CONFIG::windowHeight);
    ui->spin_opacity->setValue(DEFAULT_CONFIG::opacity);
    ui->spin_borderRadius->setValue(DEFAULT_CONFIG::borderRadius);
    font = DEFAULT_CONFIG::font;
    ui->label_font->setText(
        QString("%1 %2").arg(font.family(), font.styleName()));
    ui->spin_fontSize->setValue(font.pointSize());
    mainColor = DEFAULT_CONFIG::mainColor;
    usernameColor = DEFAULT_CONFIG::usernameColor;
    contentColor = DEFAULT_CONFIG::contentColor;
    backgroundColor = DEFAULT_CONFIG::backgroundColor;
    ui->spin_scrollingSpeed->setValue(DEFAULT_CONFIG::scrollingSpeed);
    ui->combo_fps->setCurrentText(QString::number(DEFAULT_CONFIG::fps));
    ui->slider_fps->setValue(
        ui->combo_fps->findText(ui->combo_fps->currentText()));

#define SET_BTN_STYLE_SHEET(NAME)  \
    ui->btn_##NAME->setStyleSheet( \
        QString("background-color:%1").arg(NAME.name()))

    SET_BTN_STYLE_SHEET(mainColor);
    SET_BTN_STYLE_SHEET(usernameColor);
    SET_BTN_STYLE_SHEET(contentColor);
    SET_BTN_STYLE_SHEET(backgroundColor);

#undef SET_BTN_STYLE_SHEET
}

void DanmuPanel::testDanmu() {
    emit testDanmu(14003442, QString("XuanYun_Fidel"),
                   QString("Danmu test Danmu test Danmu test Danmu test."),
                   true, false, 233);
}

void DanmuPanel::setFont() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this->font, this);
    if (ok) {
        this->font = font;
        ui->label_font->setText(
            QString("%1 %2").arg(font.family(), font.styleName()));
        ui->spin_fontSize->setValue(font.pointSize());
    }
}

void DanmuPanel::setFontSize(const int &fontSize) {
    font.setPointSize(fontSize);
}

#define SET_COLOR(NAME)                        \
    NAME = QColorDialog::getColor(NAME, this), \
    ui->btn_##NAME->setStyleSheet(             \
        QString("background-color:%1").arg(NAME.name()))

void DanmuPanel::setMainColor() { SET_COLOR(mainColor); }

void DanmuPanel::setUsernameColor() { SET_COLOR(usernameColor); }

void DanmuPanel::setContentColor() { SET_COLOR(contentColor); }

void DanmuPanel::setBackgroundColor() { SET_COLOR(backgroundColor); }

#undef SET_COLOR