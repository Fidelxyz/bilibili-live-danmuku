#include "mainwindow.h"

#include <vector>

#include "module.h"
#include "modules/danmu_display/danmu_display.h"
#include "modules/live_room/live_room.h"
#include "ui_mainwindow.h"

class Module;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    qSetMessagePattern("%{if-category}%{category} | %{endif}%{type} | %{file}:%{line} | %{function} | %{message}");

    layout_modules = new QVBoxLayout(ui->centralWidget);

    Module::moduleManager = this;

    addModule(new LiveRoom());
    addModule(new DanmuDisplay());
}

MainWindow::~MainWindow() {
    qDebug("~MainWindow");
}

Module *MainWindow::getModule(const QString &name) const {
    auto iter = modules.find(name);
    if (iter == modules.end()) {
        qWarning() << "Module" << name << "not found.";
        return nullptr;
    }
    return iter->second;
}

void MainWindow::addModule(Module *module) {
    qDebug() << "Adding module:" << module->moduleMetadata.name << module;
    modules.insert(std::make_pair(module->moduleMetadata.name, module));
    layout_modules->addWidget(module->getWidget());
}

/*
Dependency:
liveRoom <- danmuDisplay <- danmuPanel

*/