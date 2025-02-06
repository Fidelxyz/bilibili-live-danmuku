#ifndef DANMUKU_MODULES_DANMU_DISPLAY_DANMU_WINDOW_H_
#define DANMUKU_MODULES_DANMU_DISPLAY_DANMU_WINDOW_H_

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class DanmuWindow;
}
QT_END_NAMESPACE

class DanmuWindow final : public QWidget {
    Q_OBJECT

   public:
    explicit DanmuWindow(QWidget *parent = nullptr);
    ~DanmuWindow() override;
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

#endif  // DANMUKU_MODULES_DANMU_DISPLAY_DANMU_WINDOW_H_