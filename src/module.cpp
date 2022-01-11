#include "module.h"

#include <QDebug>

#include "danmuku.h"

Danmuku *Module::danmuku = nullptr;

Module::Module(const QString &name, const QList<QString> &dependencies)
    : QObject(danmuku) {
    qDebug() << "Loading module:" << name;
    moduleMetadata.name = name;
    moduleMetadata.dependencies = dependencies;

    widget = new QWidget(danmuku);  // deleted by QT
    // TODO check dependencies
}

Module::~Module() { qDebug() << "Unloading module:" << moduleMetadata.name; }

QWidget *Module::getWidget() const { return widget; }

Module *Module::getModule(const QString &name) {
    return danmuku->getModule(name);
}