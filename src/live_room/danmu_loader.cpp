#include "danmu_loader.h"

DanmuLoader::DanmuLoader(QListWidget *list) {
    this->list = list;
    scrollBar = list->verticalScrollBar();
    listViewHeight = list->height();
    loadingItemTotalHeight = 0;
    scrollingSpeed = 0;
    sleepIntervalMs = 0;
}

DanmuLoader::~DanmuLoader() {
    updateMutex.lock();
    requestInterruption();
    updateCondition.wakeAll();
    updateMutex.unlock();
    wait();
}

void DanmuLoader::run() {
    updateMutex.lock();
    while (!isInterruptionRequested()) {
        while (!loadingItemQueue.empty()) {
            int scrollBarPos = scrollBar->value();
            scrollBarPos += loadingItemTotalHeight * scrollingSpeed;
            // qDebug() << "Scroll to position: " << scrollBarPos;
            scrollBar->setValue(scrollBarPos);

            // OUT
            int removeCount = 0;
            for (int i = 0;; i++) {
                QRect region = list->visualItemRect(
                    list->item(i));  // visualItemRect warn ?
                int height = region.height();
                if (region.top() + height <= 0) {
                    qDebug() << "Delete: " << i;
                    removeCount++;
                    scrollBarPos -= height;
                } else {
                    break;
                }
            }

            if (removeCount > 0) {
                for (; removeCount; removeCount--) {
                    QListWidgetItem *item = list->takeItem(0);
                    list->removeItemWidget(item);
                    delete item;
                }
                // qDebug() << "Scroll to position: " << scrollBarPos;
                scrollBar->setValue(scrollBarPos);
            }

            // IN
            while (!loadingItemQueue.empty()) {
                QRect region = list->visualItemRect(loadingItemQueue.front());
                int height = region.height();
                if (region.top() + height <= listViewHeight) {
                    loadingItemQueue.pop();
                    loadingItemTotalHeight -= height;
                } else {
                    break;
                }
            }
            msleep(sleepIntervalMs);
        }
        updateCondition.wait(&updateMutex);
    }
    updateMutex.unlock();
}

void DanmuLoader::loadItem(QListWidgetItem *item) {
    loadingItemQueue.push(item);
    loadingItemTotalHeight += item->sizeHint().height();
    updateCondition.wakeAll();
}

void DanmuLoader::setScrollingSpeed(const float &speed, const int &fps) {
    scrollingSpeed = speed / fps;
    sleepIntervalMs = 1000 / fps;
}