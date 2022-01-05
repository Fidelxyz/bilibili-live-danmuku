#include "danmu_loader.h"

DanmuLoader::DanmuLoader(QListWidget *list) {
    this->list = list;
    scrollBar = list->verticalScrollBar();
    listViewHeight = list->height();
    loadingItemTotalHeight = 0;
    loadedItemTotalHeight = listViewHeight;
    scrollBarPos = 0;
    scrollingSpeed = 0;
    sleepIntervalMs = 0;
}

DanmuLoader::~DanmuLoader() {
    requestInterruption();
    updateMutex.lock();
    updateCondition.wakeAll();
    updateMutex.unlock();
    wait();
}

void DanmuLoader::slotReload() {
    QMutexLocker loopLocker(&loopMutex);

    // clear queue
    std::queue<QListWidgetItem *> empty;
    std::swap(loadingItemQueue, empty);

    listViewHeight = list->height();
    loadingItemTotalHeight = 0;
    loadedItemTotalHeight = listViewHeight;
    scrollBarPos = 0;
    updateCondition.wakeAll();
}

void DanmuLoader::run() {
    QMutexLocker updateLocker(&updateMutex);
    while (!isInterruptionRequested()) {
        while (true) {
            QMutexLocker loopLocker(&loopMutex);
            if (loadingItemQueue.empty()) {
                break;
            }
            scrollBarPos += loadingItemTotalHeight * scrollingSpeed;

            // OUT
            while (true) {
                int itemHeight = list->item(0)->sizeHint().height();
                if (itemHeight + list->item(1)->sizeHint().height() <
                    scrollBarPos) {
                    qDebug("Delete item.");
                    scrollBarPos -= itemHeight;
                    loadedItemTotalHeight -= itemHeight;
                    QListWidgetItem *item = list->takeItem(0);
                    delete list->itemWidget(item);
                    delete item;
                } else {
                    break;
                }
            }
            scrollBar->setValue(scrollBarPos);

            // IN
            while (!loadingItemQueue.empty()) {
                int itemHeight = loadingItemQueue.front()->sizeHint().height();
                if (loadedItemTotalHeight + itemHeight <=
                    scrollBarPos + listViewHeight) {
                    loadingItemQueue.pop();
                    loadingItemTotalHeight -= itemHeight;
                    loadedItemTotalHeight += itemHeight;
                } else {
                    break;
                }
            }
            loopLocker.unlock();
            msleep(sleepIntervalMs);
        }
        updateCondition.wait(&updateMutex);
    }
}

void DanmuLoader::loadItem(QListWidgetItem *item) {
    loopMutex.lock();
    loadingItemQueue.push(item);
    loadingItemTotalHeight += item->sizeHint().height();
    loopMutex.unlock();
    updateCondition.wakeAll();
}

void DanmuLoader::slotSetScrollingSpeed(const float &speed, const int &fps) {
    scrollingSpeed = speed / fps;
    sleepIntervalMs = 1000 / fps;
}