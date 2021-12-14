#ifndef DANMU_LOADER_H
#define DANMU_LOADER_H

#include <QListWidget>
#include <QMutex>
#include <QScrollBar>
#include <QThread>
#include <QWaitCondition>
#include <queue>

class DanmuLoader : public QThread {
    Q_OBJECT

   public:
    DanmuLoader(QListWidget *list);
    ~DanmuLoader();
    void loadItem(QListWidgetItem *item);

   protected:
    void run() override;

   public slots:
    void setScrollingSpeed(const float &speed, const int &fps);

   private:
    std::queue<QListWidgetItem *> loadingItemQueue;  // item, height
    int loadingItemTotalHeight;
    int listViewHeight;
    float scrollingSpeed;
    unsigned long sleepIntervalMs;

    QListWidget *list;
    QScrollBar *scrollBar;
    QMutex updateMutex;
    QWaitCondition updateCondition;
};

#endif