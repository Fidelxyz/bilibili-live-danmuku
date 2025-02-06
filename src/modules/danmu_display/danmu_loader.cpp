#include "danmu_loader.h"

DanmuLoader::DanmuLoader(QListWidget *list, QObject *parent) : QThread(parent) {
    this->list = list;
    list->installEventFilter(this);
    scrollBar              = list->verticalScrollBar();
    listViewHeight         = list->height();
    loadingItemTotalHeight = 0;
    loadedItemTotalHeight  = listViewHeight;
    scrollBarPos           = 0;
    scrollingSpeed         = 0;
    sleepIntervalMs        = 0;
}

DanmuLoader::~DanmuLoader() {
    requestInterruption();
    updateMutex.lock();
    updateCondition.wakeAll();
    updateMutex.unlock();
    wait();
}

void DanmuLoader::reload() {
    QMutexLocker loopLocker(&loopMutex);

    list->clear();

    auto *blankItem = new QListWidgetItem(list);  // deleted by QT
    blankItem->setSizeHint(list->size());
    qDebug() << "size:" << list->size();
    list->addItem(blankItem);

    // clear queue
    std::queue<QListWidgetItem *> empty;
    std::swap(loadingItemQueue, empty);

    listViewHeight         = list->height();
    loadingItemTotalHeight = 0;
    loadedItemTotalHeight  = listViewHeight;
    scrollBarPos           = 0;

    updateCondition.wakeAll();
}

bool DanmuLoader::eventFilter(QObject *obj, QEvent *event) {
    if (obj == list) {
        if (event->type() == QEvent::Resize) {
            qDebug() << "eventFilter: Resize";
            QMetaObject::invokeMethod(this, "reload");
        }
    }
    return QThread::eventFilter(obj, event);
}

void DanmuLoader::run() {
    QMutexLocker updateLocker(&updateMutex);
    while (!isInterruptionRequested()) {
        while (true) {
            QMutexLocker loopLocker(&loopMutex);
            if (loadingItemQueue.empty()) {
                break;
            }
            scrollBarPos += static_cast<int>(
                static_cast<float>(loadingItemTotalHeight) * scrollingSpeed);
            // qDebug() << "Scrolling speed:"
            //          << loadingItemTotalHeight * scrollingSpeed;

            // OUT
            while (true) {
                if (const int itemHeight = list->item(0)->sizeHint().height();
                    itemHeight + list->item(1)->sizeHint().height() <
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
                if (const int itemHeight =
                        loadingItemQueue.front()->sizeHint().height();
                    loadedItemTotalHeight + itemHeight <=
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

void DanmuLoader::setScrollingSpeed(const int speed, const int fps) {
    scrollingSpeed  = static_cast<float>(speed) / 10 / static_cast<float>(fps);
    sleepIntervalMs = 1000 / fps;
}