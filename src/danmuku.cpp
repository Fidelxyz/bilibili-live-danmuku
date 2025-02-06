#include "danmuku.h"

#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <vector>

#include "modules/danmu_display/danmu_display.h"
#include "modules/live_room/live_room.h"

Danmuku::Danmuku(QWidget *parent) : QMainWindow(parent) {  // deleted in ~Danmuku
    qSetMessagePattern(
        "%{if-category}%{category} | %{endif}%{type} | %{file}:%{line} | "
        "%{function} | %{message}");

    // setup UI
    resize(300, 180);
    setWindowTitle(tr("Bilibili弹幕库 Developed by Fidel"));

    centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setEnabled(true);
    setCentralWidget(centralWidget);

    layout_modules = new QVBoxLayout(centralWidget);  // deleted by QT

    // TODO: Dynamic modules loader
    loadModule(new LiveRoom(this));      // deleted by Qt
    loadModule(new DanmuDisplay(this));  // deleted by Qt

    // Qss
    QFile qss(":/stylesheet/danmuku.qss");
    if (qss.open(QFile::ReadOnly)) {
        qDebug("QSS loaded.");
        QString stylesheet = QLatin1String(qss.readAll());
        setStyleSheet(stylesheet);
        qss.close();
    } else {
        qWarning("Failed to load QSS stylesheet (danmuku.qss).");
    }
}

Danmuku::~Danmuku() {
    qDebug("Enter ~Danmuku");
    for (auto module = modules.begin(); module != modules.end();
         module = modules.erase(module)) {
        unloadModule(module->second);
    }
    qDebug("Exit ~Danmuku");
}

Module *Danmuku::getModule(const QString &name) const {
    auto iter = modules.find(name);
    if (iter == modules.end()) {
        qWarning() << "Module" << name << "not found.";
        return nullptr;
    }
    return iter->second;
}

void Danmuku::loadModule(Module *module) {
    qDebug() << "Loading module:" << module->moduleMetadata.name << module;
    modules.insert(std::make_pair(module->moduleMetadata.name, module));
    layout_modules->addWidget(module->getWidget());
}

void Danmuku::unloadModule(Module *module) {
    qDebug() << "Unloading module:" << module->moduleMetadata.name << module;
    layout_modules->removeWidget(module->getWidget());
    delete module;
}