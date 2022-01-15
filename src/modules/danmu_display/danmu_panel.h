#ifndef DANMU_PANEL_H
#define DANMU_PANEL_H

#include <QWidget>

#include "danmu_config.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DanmuPanel;
}
QT_END_NAMESPACE

class DanmuPanel : public QWidget {
    Q_OBJECT

   public:
    DanmuPanel(DanmuConfig *config, QWidget *parent = nullptr);
    ~DanmuPanel();

   private:
    void emitTestDanmu();
    void emitTestGift();
    void loadConfig();
    void apply();
    void setToDefault();
    void setFont();
    void setFontSize(const int &fontSize);
    void setMainColor();
    void setUsernameColor();
    void setContentColor();
    void setBackgroundColor();
    void toggleShowGift();
    void setShowGift(const bool &on);

   signals:
    void testDanmu(const int &uid, const QString &username, const QString &text,
                   const bool &isAdmin, const bool &isVIP,
                   const int &userGuardLevel);
    void testGift(const int &uid, const QString &username,
                  const QString &giftName, const int &giftCount);

   private:
    Ui::DanmuPanel *ui;

    DanmuConfig *config;

    QFont font;
    QColor mainColor;
    QColor usernameColor;
    QColor contentColor;
    QColor backgroundColor;

    bool showGift;
};

#endif