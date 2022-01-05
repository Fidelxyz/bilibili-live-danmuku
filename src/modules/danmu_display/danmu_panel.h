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
    void loadConfig();

   signals:
    void testDanmu(const int &uid, const QString &username, const QString &text,
                   const bool &isAdmin, const bool &isVIP,
                   const int &userGuardLevel);

   private slots:
    void slotApply();
    void slotTestDanmu();
    void slotSetMainColor();
    void slotSetUsernameColor();
    void slotSetContentColor();
    void slotSetBackgroundColor();

   private:
    Ui::DanmuPanel *ui;

    DanmuConfig *config;

    QColor mainColor;
    QColor usernameColor;
    QColor contentColor;
    QColor backgroundColor;
};

#endif