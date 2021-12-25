#ifndef DANMU_PANEL_H
#define DANMU_PANEL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class DanmuPanel;
}
QT_END_NAMESPACE

class DanmuPanel : public QWidget {
    Q_OBJECT

   public:
    DanmuPanel(QWidget *parent = nullptr);
    ~DanmuPanel();

   signals:
    void getWindowConfig(int *width, int *height, int *opacity);
    void getFontConfig(int *fontSize);
    void getColorConfig(QColor *mainColor, QColor *usernameColor,
                        QColor *contentColor, QColor *backgroundColor);
    void setWindowConfig(const int &width, const int &height,
                         const int &opacity);
    void setFontConfig(const int &fontSize);
    void setColorConfig(const QColor &mainColor, const QColor &usernameColor,
                        const QColor &contentColor,
                        const QColor &backgroundColor);
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
    void slotGetConfig();

   private:
    Ui::DanmuPanel *ui;
    QColor mainColor;
    QColor usernameColor;
    QColor contentColor;
    QColor backgroundColor;
};

#endif