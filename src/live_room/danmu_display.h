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
    void slotRecvDanmuDebug();

   private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

   private:
    Ui::DanmuDisplay *ui;
    DanmuLoader *danmuLoader;

    // Position
    bool isMouseLeftPressed;
    QPoint mouseDragPosition;
};

#endif