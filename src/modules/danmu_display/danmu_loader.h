#ifndef DANMUKU_MODULES_DANMU_DISPLAY_DANMU_LOADER_H_
#define DANMUKU_MODULES_DANMU_DISPLAY_DANMU_LOADER_H_

#include <QEvent>
#include <QListWidget>
#include <QMutex>
#include <QScrollBar>
#include <QThread>
#include <QWaitCondition>
#include <queue>

class DanmuLoader : public QThread {
    Q_OBJECT

   public:
    DanmuLoader(QListWidget *list, QObject *parent = nullptr);
    ~DanmuLoader();
    void loadItem(QListWidgetItem *item);

   protected:
    void run() override;
    bool eventFilter(QObject *obj, QEvent *event) override;

   public slots:
    void reload();
    void setScrollingSpeed(const float &speed, const int &fps);

   private:
    std::queue<QListWidgetItem *> loadingItemQueue;  // item, height
    int loadingItemTotalHeight;
    int loadedItemTotalHeight;
    int listViewHeight;
    float scrollBarPos;
    float scrollingSpeed;
    unsigned long sleepIntervalMs;

    QListWidget *list;
    QScrollBar *scrollBar;
    QMutex updateMutex;
    QMutex loopMutex;
    QWaitCondition updateCondition;
};

#endif  // DANMUKU_MODULES_DANMU_DISPLAY_DANMU_LOADER_H_