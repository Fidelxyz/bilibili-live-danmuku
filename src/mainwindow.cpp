#include "mainwindow.h"

#include <vector>

#include "modules/danmu_display/danmu_display.h"
#include "modules/live_room/live_room.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    layout_modules = new QVBoxLayout(ui->centralWidget);

    addModule(new LiveRoom(this));
    addModule(new DanmuDisplay(this));
}

MainWindow::~MainWindow() {}

Module *MainWindow::getModule(const QString &name) const {
    auto iter = modules.find(name);
    if (iter == modules.end()) {
        return nullptr;
    }
    return iter->second;
}

void MainWindow::addModule(Module *module) {
    modules.insert(std::make_pair(module->moduleMetadata.name, module));
    layout_modules->addWidget(module->getWidget());
}

/*
Dependency:
liveRoom <- danmuDisplay <- danmuPanel

*/