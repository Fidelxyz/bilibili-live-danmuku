#include "module.h"

#include "danmuku.h"

Module::Module(const QString &name, const QList<QString> &dependencies,
               Danmuku *parent)
    : QObject(parent) {
    qDebug() << "Loading module:" << name;
    moduleMetadata.name         = name;
    moduleMetadata.dependencies = dependencies;

    widget = new QWidget(parent);  // deleted by Qt
    // TODO check dependencies
}

Module::~Module() { qDebug() << "Unloading module:" << moduleMetadata.name; }

QWidget *Module::getWidget() const { return widget; }

// for communication with other modules
Module *Module::getModule(const QString &name) const {
    Q_ASSERT(moduleMetadata.dependencies.contains(name));
    return dynamic_cast<Danmuku *>(parent())->getModule(name);
}