#include "danmuku.h"

#include <vector>

#include "module.h"
#include "modules/danmu_display/danmu_display.h"
#include "modules/live_room/live_room.h"
#include "ui_danmuku.h"

class Module;

Danmuku::Danmuku(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Danmuku) {  // deleted in ~Danmuku
    ui->setupUi(this);

    qSetMessagePattern(
        "%{if-category}%{category} | %{endif}%{type} | %{file}:%{line} | "
        "%{function} | %{message}");

    layout_modules = new QVBoxLayout(ui->centralWidget);  // deleted by QT

    Module::danmuku = this;
    loadModule(new LiveRoom());      // deleted in unloadModule
    loadModule(new DanmuDisplay());  // deleted in unloadModule
}

Danmuku::~Danmuku() {
    qDebug("Enter ~Danmuku");
    for (auto module = modules.begin(); module != modules.end();
         module = modules.erase(module)) {
        unloadModule(module->second);
    }
    delete ui;
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
    delete module;
}