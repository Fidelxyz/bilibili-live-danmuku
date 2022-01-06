#include "module.h"

#include <QDebug>

#include "mainwindow.h"

MainWindow *Module::moduleManager = nullptr;

Module::Module(const QString &name, const QList<QString> &dependencies)
    : QObject(moduleManager) {
    qDebug() << "Loading module:" << name;
    moduleMetadata.name = name;
    moduleMetadata.dependencies = dependencies;

    widget = new QWidget(moduleManager);
    // TODO check dependencies
}

Module::~Module() { qDebug() << "Unloading module:" << moduleMetadata.name; }

QWidget *Module::getWidget() const { return widget; }

Module *Module::getModule(const QString &name) {
    return moduleManager->getModule(name);
}