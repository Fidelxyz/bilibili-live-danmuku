#include "danmu_panel.h"

#include <QColorDialog>
#include <QFontDialog>

#include "ui/ui_danmu_panel.h"

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

    connect(ui->btn_font, &QPushButton::clicked, this, &DanmuPanel::setFont);
    connect(ui->spin_fontSize, &QSpinBox::valueChanged, this,
            &DanmuPanel::setFontSize);
    connect(ui->btn_mainColor, &QPushButton::clicked, this,
            &DanmuPanel::setMainColor);
    connect(ui->btn_usernameColor, &QPushButton::clicked, this,
            &DanmuPanel::setUsernameColor);
    connect(ui->btn_contentColor, &QPushButton::clicked, this,
            &DanmuPanel::setContentColor);
    connect(ui->btn_backgroundColor, &QPushButton::clicked, this,
            &DanmuPanel::setBackgroundColor);

    ui->combo_fps->addItems({"15", "30", "60"});

    connect(ui->btn_apply, &QPushButton::clicked, this, &DanmuPanel::apply);
    connect(ui->btn_setToDefault, &QPushButton::clicked, this,
            &DanmuPanel::setToDefault);
    connect(ui->btn_cancelChange, &QPushButton::clicked, this,
            &DanmuPanel::loadConfig);

    connect(ui->btn_testDanmu, &QPushButton::clicked, this,
            &DanmuPanel::emitTestDanmu);
    connect(ui->btn_testGift, &QPushButton::clicked, this,
            &DanmuPanel::emitTestGift);

    connect(ui->btn_toggleShowGift, &QPushButton::clicked, this,
            &DanmuPanel::toggleShowGift);

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
    showGift = config->showGift;
    setShowGift(showGift);
    ui->spin_giftHeightRatio->setValue(config->giftHeightRatio);

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
    config->showGift = showGift;
    config->giftHeightRatio = ui->spin_giftHeightRatio->value();
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
    showGift = DEFAULT_CONFIG::showGift;
    setShowGift(showGift);
    ui->spin_giftHeightRatio->setValue(DEFAULT_CONFIG::giftHeightRatio);

#define SET_BTN_STYLE_SHEET(NAME)  \
    ui->btn_##NAME->setStyleSheet( \
        QString("background-color:%1").arg(NAME.name()))

    SET_BTN_STYLE_SHEET(mainColor);
    SET_BTN_STYLE_SHEET(usernameColor);
    SET_BTN_STYLE_SHEET(contentColor);
    SET_BTN_STYLE_SHEET(backgroundColor);

#undef SET_BTN_STYLE_SHEET
}

void DanmuPanel::emitTestDanmu() {
    emit testDanmu(14003442, "XuanYun_Fidel",
                   "Danmu test Danmu test Danmu test Danmu test.", true, true,
                   233);
}

void DanmuPanel::emitTestGift() {
    emit testGift(14003442, "XuanYun_Fidel", "原石", 160);
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

void DanmuPanel::toggleShowGift() {
    showGift = !showGift;
    setShowGift(showGift);
}

void DanmuPanel::setShowGift(const bool &on) {
    ui->btn_toggleShowGift->setText(on ? "[ON]" : "[OFF]");
    ui->slider_giftHeightRatio->setEnabled(on);
    ui->spin_giftHeightRatio->setEnabled(on);
}

#undef SET_COLOR