#ifndef DANMU_WINDOW_H
#define DANMU_WINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class DanmuWindow;
}
QT_END_NAMESPACE

class DanmuWindow : public QWidget {
    Q_OBJECT

   public:
    DanmuWindow(QWidget *parent = nullptr);
    ~DanmuWindow();
    void setTransparentForMouseEvents(const bool &on);

   private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

   public:
    Ui::DanmuWindow *ui;

   private:
    bool isMouseLeftPressed;
    QPoint mouseDragPosition;
};

#endif