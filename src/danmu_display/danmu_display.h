#ifndef DANMU_DISPLAY_H
#define DANMU_DISPLAY_H

#include <QListWidget>

#include "danmu_loader.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DanmuDisplay;
}
QT_END_NAMESPACE

class DanmuDisplay : public QWidget {
    Q_OBJECT

   public:
    DanmuDisplay(QWidget *parent = nullptr);
    ~DanmuDisplay();

   public slots:
    void slotRecvDanmu(const int &uid, const QString &username,
                       const QString &text, const bool &isAdmin,
                       const bool &isVIP, const int &userGuardLevel);
    void slotUpdateViewersCount(const int &viewersCount);
    void slotUpdateFollowersCount(const int &followersCount);

    void slotGetWindowConfig(int *width, int *height, int *opacity);
    void slotGetFontConfig(int *fontSize);
    void slotGetColorConfig(QColor *mainColor, QColor *usernameColor, QColor *contentColor,
                            QColor *backgroundColor);

    void slotSetWindowConfig(const int &width, const int &height,
                             const int &opacity);
    void slotSetFontConfig(const int &fontSize);
    void slotSetColorConfig(const QColor &mainColor, const QColor &usernameColor,
                            const QColor &contentColor,
                            const QColor &backgroundColor);

   private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void reload();
    void setMainStyle();

   private:
    Ui::DanmuDisplay *ui;
    DanmuLoader *danmuLoader;

    // window position
    bool isMouseLeftPressed;
    QPoint mouseDragPosition;

    // style
    int fontSize;
    QString mainColor;
    QString usernameColor;
    QString contentColor;
    QString backgroundColor;
};

#endif