#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "danmu_display/danmu_display.h"
#include "danmu_display/danmu_panel.h"
#include "live_room/live_room.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

   public slots:
    void slotStartLiveRoom();
    void slotStartDanmuDisplay();
    void slotStartDanmuPanel();

   private:
    Ui::MainWindow* ui;
    LiveRoom* liveRoom;
    DanmuDisplay* danmuDisplay;
    DanmuPanel* danmuPanel;
};

#endif